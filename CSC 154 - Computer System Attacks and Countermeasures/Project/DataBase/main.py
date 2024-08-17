from flask import Flask, render_template, request, redirect, url_for, session, jsonify
import sqlite3
import os
import hashlib
from cryptography.fernet import Fernet
import base64
import re
import requests
import random 
from functools import wraps





app = Flask(__name__)
app.secret_key = app.secret_key = os.getenv('SECRET_KEY', 'default_secret_key')



FILES_DIR = 'files'
USER_DIR = 'users'

def table_creation(cur):
    
    


    cur.execute("CREATE TABLE IF NOT EXISTS users ("
                "key INTEGER PRIMARY KEY AUTOINCREMENT, "
                "role_id INTEGER, "
                "name TEXT UNIQUE NOT NULL, "
                "password TEXT NOT NULL,"
                "phone TEXT NOT NULL,"
                "phoneKey TEXT NOT NULL"
                ")")

    cur.execute("CREATE TABLE IF NOT EXISTS files ("
                "filenum INTEGER PRIMARY KEY AUTOINCREMENT, "
                "filename TEXT UNIQUE NOT NULL, "
                "owner_id INTEGER NOT NULL,"
                "owner_role_id INTEGER NOT NULL,"
                "view_by_manager BOOLEAN NOT NULL DEFAULT FALSE, "
                "view_by_user TEXT DEFAULT '', "
                "FOREIGN KEY (owner_id) REFERENCES users(key), "
                "FOREIGN KEY (view_by_user) REFERENCES users(key))")

def get_db_connection():
    conn = sqlite3.connect('rbac.db')
    conn.row_factory = sqlite3.Row
    return conn

def key_db():
    conn = sqlite3.connect('key.db')
    conn.row_factory = sqlite3.Row
    return conn


id_dir = {"manager": 1, "human resources": 2, "engineer": 3}

#Role Id look up
def id_search(cur, role_name):
    cur.execute("SELECT key FROM users WHERE role_id = ?", (id_dir[role_name.lower()],))
    return cur.fetchone()

#Password hashing
def hash_password(password):
    return hashlib.sha256(password.encode()).hexdigest()

#Encryption
def encrypt(phone): #Encrypts and returns a key along with the encrypted phone, also translates it to base64 for save SQL storing
    key = Fernet.generate_key()
    fernet = Fernet(key)
    encryptedPhone = fernet.encrypt(phone.encode())
    return base64.urlsafe_b64encode(encryptedPhone), base64.urlsafe_b64encode(key)

#Decryption using key and base64 for save SQL storing
def decrypt(key, phone):
    key = base64.urlsafe_b64decode(key)
    phone = base64.urlsafe_b64decode(phone)
    fernet = Fernet(key)
    return fernet.decrypt(phone).decode()


@app.route('/create_user', methods=['GET', 'POST'])
def create_user():
    if request.method == 'POST':
        name = request.form['name']
        password = hash_password(request.form['password'])
        phone, phoneKey = encrypt(request.form['phone'])
        role_name = request.form['role_name']
        conn = get_db_connection()
        cur = conn.cursor()

        print(decrypt(phoneKey.decode(),phone.decode()))

        cur.execute("SELECT * FROM users WHERE name = ?",(name,))
        exists = cur.fetchone()
        if exists:
            return "User already exists"
    
        if role_name.lower() in ["manager", "human resources"]:
            if id_search(cur, role_name):
                return "There can only be a single role: {}".format(role_name)
            else:
                cur.execute("INSERT INTO users (role_id, name, password, phone, phoneKey) VALUES (?, ?, ?, ?, ?)",
                            (id_dir[role_name.lower()], name, password, phone.decode(), phoneKey.decode()))
        elif role_name.lower() == "engineer":
            cur.execute("INSERT INTO users (role_id, name, password, phone, phoneKey) VALUES (?, ?, ?, ?, ?)",
                        (id_dir[role_name.lower()], name, password,phone.decode(), phoneKey.decode()))
        else:
            return "Invalid Role: {}".format(role_name)
        
        # If exists
        if not os.path.exists(USER_DIR):
            os.makedirs(USER_DIR)

        with open(os.path.join(USER_DIR, name + ".txt"), 'w') as file:
            file.write("name: %s\n" % (name))
            file.write("password: %s\n" % (password))
            file.write("phone number: %s\n" % phone)
            file.close

        conn.commit()
        conn.close()
        return redirect(url_for('index'))
    return render_template('create_user.html')

@app.route('/create_file', methods=['GET', 'POST'])
def create_file():
    if request.method == 'POST':
        filename = request.form['filename']
        owner_id = int(request.form['owner_id'])
        owner_role_id = int(request.form['owner_role_id']) 
        view_by_user = None
        view_by_manager = False
        

        conn = get_db_connection()
        cur = conn.cursor()

        if owner_role_id == 2:  # Human Resources
            view_by_manager = 'view_by_manager' in request.form
        elif owner_role_id == 1:  # Manager
            view_by_user_input = request.form.get('view_by_user')         
            cur.execute("SELECT key FROM users WHERE key != ?", (owner_id,))
            users_exist = [str(row['key']) for row in cur.fetchall()]  # Convert to strings for comparison
            if view_by_user_input:
                ids = re.split(r'[ ,]+', view_by_user_input.strip())
                valid_ids = [user_id for user_id in ids if user_id in users_exist]
                if valid_ids:
                    if view_by_user:
                        view_by_user += ',' + ','.join(valid_ids)
                    else:
                        view_by_user = ','.join(valid_ids)       
        
        cur.execute("SELECT * FROM files WHERE filename = ?",(filename,))
        exists = cur.fetchone()
        if exists:
            return "File already exists"

        cur.execute("INSERT INTO files (filename, owner_id, owner_role_id, view_by_manager, view_by_user) VALUES (?, ?, ?, ?, ?)",
                    (filename, owner_id, owner_role_id, int(view_by_manager), view_by_user))

        # If exists
        if not os.path.exists(FILES_DIR):
            os.makedirs(FILES_DIR)

        with open(os.path.join(FILES_DIR, filename + ".txt"), 'w') as file:
            file.write("")
            file.close()

        conn.commit()
        conn.close()
        return redirect(url_for('user_page', user_id=owner_id))
    
    owner_id = int(request.args.get('owner_id'))
    owner_role_id = int(request.args.get('owner_role_id'))
    return render_template('create_file.html', owner_id=owner_id, owner_role_id=owner_role_id)

@app.route('/')
def index():
    conn = get_db_connection()
    cur = conn.cursor()
    table_creation(cur)
    cur.execute("SELECT * FROM users")
    users = cur.fetchall()
    cur.execute("SELECT * FROM files")
    files = cur.fetchall()
    conn.close()
    return render_template('index.html', users=users, files=files)

def generate_code():
    return random.randint(10000, 99999)

@app.route('/verify_code', methods=['POST'])
def verify_code():
    data = request.json
    user_code = data.get('code')

    if user_code and session.get('verification_code') and user_code == str(session.get('verification_code')):
        session.pop('verification_code', None)  
        return jsonify(success=True, redirect_url=url_for('user_page', user_id=session.get('user_id')))
    return jsonify(success=False)


def login_required(f):
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if 'user_id' not in session:
            return redirect(url_for('login', next=request.url))
        return f(*args, **kwargs)
    return decorated_function

@app.route('/logout')
def logout():
    session.pop('user_id', None)
    return redirect(url_for('index'))

@app.route('/user', methods=['GET', 'POST'])
#Login
def login():
    if request.method == "POST":
        username = request.form["username"]
        password = hash_password(request.form["password"])

        conn = get_db_connection()
        cur = conn.cursor()
        cur.execute("SELECT key, password, phone, phoneKey FROM users WHERE name = ?", (username,))
        user = cur.fetchone()
        
        #Get the TEXTBELT secret key
        keyCon = key_db()
        cr = keyCon.cursor()
        cr.execute("SELECT token, key FROM tokenTable")
        encr = cr.fetchone()
        
        #If the user exists pass on to checking the password
        if user:
            if user["password"] == password and user["phone"] != None: #If password is valid
                code = generate_code()
                resp = requests.post('https://textbelt.com/text', {
                    'phone': decrypt(user['phoneKey'], user["phone"]),
                    'message': code,
                    'key': decrypt(encr["key"], encr["token"]),
                })
                print(code)
                session['verification_code'] = code  #Add user_id to the session so we could access without relogging just by using a link
                session['user_id'] = user['key']
                
                return render_template('index.html', show_verification=True) 

            return "Password is incorrect"                                      
        else:
            return "No user found"
    return redirect(url_for('index'))

@app.route('/user/<int:user_id>')
@login_required

#User page
def user_page(user_id):
    if session.get('user_id') != user_id:
        return "Unauthorized", 403
    
    conn = get_db_connection()
    cur = conn.cursor()
    cur.execute("SELECT * FROM users WHERE key = ?", (user_id,))
    user = cur.fetchone()
    files_viewable = None


    #Filter out the files, based on the role 
    if user["role_id"] == id_dir["engineer"]:
        cur.execute("SELECT * FROM files WHERE (owner_role_id = ? OR (',' || view_by_user || ',' LIKE ?)) AND owner_id != ?", ((id_dir["engineer"], f'%,{user_id},%',user_id)))
        files_viewable = cur.fetchall()
        cur.execute("SELECT * FROM files WHERE owner_id = ?", (user_id,))
    elif user["role_id"] == id_dir["manager"]:
        cur.execute("SELECT * FROM files WHERE view_by_manager = ?", (True,))
        files_viewable = cur.fetchall()
        cur.execute("SELECT * FROM files WHERE owner_id = ?", (user_id, ))
    elif user["role_id"] == id_dir["human resources"]:
        cur.execute("SELECT * FROM files WHERE (',' || view_by_user || ',' LIKE ?) AND owner_id != ?", (f'%,{user_id},%', user_id))
        files_viewable = cur.fetchall()
        cur.execute("SELECT * FROM files WHERE owner_id = ?", (user_id, ))

    files = cur.fetchall()
    conn.close()
    return render_template('user.html', username=user["name"], files=files, files_viewable = files_viewable, key=user["key"], role_id=user["role_id"])

@app.route('/files/<filename>/<int:user_id>')
def view_file(filename, user_id):
    try:
        # Read the file content
        with open(os.path.join(FILES_DIR, filename + ".txt"), 'r') as file:
            content = file.read()

        conn = get_db_connection()
        cur = conn.cursor()

        # Get the user's role_id
        cur.execute("SELECT * FROM users WHERE key = ?", (user_id,))
        user = cur.fetchone()
        role_id = user['role_id']
        print(f"Viewing file {filename} by user {user['name']} with role_id {role_id}")  # Debugging

        # Get the file data, including view_by_manager and view_by_user
        cur.execute("SELECT * FROM files WHERE filename = ?", (filename,))
        file_data = cur.fetchone()
        
        # Initialize available_list and user_names
        available_list = file_data["view_by_user"]
        user_names = []
        
        if available_list:
            available_list = available_list.split(",")
            for i in available_list:
                cur.execute("SELECT name FROM users WHERE key = ?", (int(i),))
                user_name = cur.fetchone()
                if user_name:
                    user_names.append(user_name["name"])
        else:
            available_list = []  

        conn.close()

        return render_template('view_file.html', filename=filename, content=content, user_id=user_id, role_id=role_id,
                               file=file_data, owned_by=file_data["owner_id"], available_list=available_list, user_names=user_names)
    
    except FileNotFoundError:
        return "File not found", 404

@app.route('/save_edit', methods=['POST'])
def save_edit():
    filename = request.form['filename']
    user_id = request.form['user_id']
    content = request.form['text']
    role_id = int(request.form['role_id'])

    conn = get_db_connection()
    cur = conn.cursor()

    if role_id == 2:  # For human resources
        view_by_manager = 'view_by_manager' in request.form
        cur.execute("UPDATE files SET view_by_manager = ? WHERE filename = ?", (int(view_by_manager), filename))
    # Save the file content
    try:
        with open(os.path.join(FILES_DIR, filename + ".txt"), 'w') as file:
            file.write(content)
        conn.commit()
        conn.close()
        return redirect(url_for('user_page', user_id=user_id))
    except FileNotFoundError:
        return "File not found", 404
    

@app.route('/add_user', methods = ['POST'])
def add_user():
    filename = request.form['filename']
    user_id = request.form['user_id']
    
    conn = get_db_connection()
    cur = conn.cursor()
    
    cur.execute("SELECT key FROM users WHERE key != ?", (user_id,))
    users_exist = [str(row['key']) for row in cur.fetchall()]  # Convert to strings for comparison
    
    cur.execute("SELECT view_by_user FROM files WHERE filename = ?", (filename,))
    result = cur.fetchone()
    users_available = result["view_by_user"] if result and result["view_by_user"] else ""
    view_by_user_input = request.form.get('view_by_user')
    
    if view_by_user_input:
        ids = re.split(r'[ ,]+', view_by_user_input.strip())
        valid_ids = [user_id for user_id in ids if user_id in users_exist]
        exist = users_available.split(',') if users_available else []
        unique_ids = list(set(valid_ids) - set(exist))
        if unique_ids:
            if users_available:
                users_available += ',' + ','.join(unique_ids)
            else:
                users_available = ','.join(unique_ids)
    cur.execute("UPDATE files SET view_by_user = ? WHERE filename = ?", (users_available, filename))
    print(type(users_available))
    conn.commit()
    conn.close()
    return redirect(request.referrer)

#Delete a file 
@app.route('/delete_file/<filename>', methods=['GET', 'POST'])
def delete_file(filename):
    conn = get_db_connection()
    cur = conn.cursor()
    cur.execute("DELETE FROM files WHERE filename = ?", (filename,))
    conn.commit()
    conn.close()

    file_path = os.path.join(FILES_DIR, filename + ".txt")
    if os.path.exists(file_path):
        os.remove(file_path)

    return redirect(request.referrer)


#Delete user from accessing a file
@app.route('/delete_user', methods=['POST'])
def delete_user():
    filename = request.form['filename']
    user_to_delete = request.form['user_to_delete']
    
    conn = get_db_connection()
    cur = conn.cursor()
    
    cur.execute("SELECT * FROM files WHERE filename = ?", (filename,))
    file_data = cur.fetchone()
    
    if file_data:
        owner_id = file_data["owner_id"]
        users = file_data["view_by_user"]
        
        if users:
            users_list = users.split(",")
            if str(user_to_delete) in users_list:
                users_list.remove(str(user_to_delete))
            
            updated_users = ",".join(users_list)
            cur.execute("UPDATE files SET view_by_user = ? WHERE filename = ?", (updated_users, filename))
            
            conn.commit()
    
    conn.close()

    return redirect(request.referrer)





if __name__ == '__main__':
    app.run(debug=True)