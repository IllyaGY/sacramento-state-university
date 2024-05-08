import sys, re, bfparse

def interpret(t):
    
    def _interpret(t):
        nonlocal data, idx  # Use data/idx from enclosing scope
        label = t.data
        if label == "+":
            data[idx] += 1
        elif label == "-":
            data[idx] -= 1
        elif label == ">":
            idx += 1
            if idx >= len(data):
                data.append(0)
        elif label == "<":
            idx -= 1
        elif label == ".":
            print(chr(data[idx]), end='')
        elif label == ",":
            data[idx] = ord(sys.stdin.read(1))
        elif label == "S":
            _interpret(t.children[0])
            if (len(t.children) == 2):
                _interpret(t.children[1])
        elif label == "C":
            _interpret(t.children[0])
        elif label == "L":
            while (data[idx] != 0):
                _interpret(t.children[1])

    data = [0]
    idx = 0
    _interpret(t)

def output(n):
    if n.data == "":
        print('λ', end='')
    else:
        print(n.data, end='')
    if not n.is_leaf():
        print("(", end='')
        for ch in n.children:
            output(ch)
        print(")", end='')

def main():
    file = open('bf.txt')
    text = file.read()
    prog = re.sub(r'[^-+<>,\.\[\]]', '', text)
    tree = bfparse.parse(prog)
    print("Program source:")
    print(prog)
    print("\nProgram tree:")
    output(tree)
    print("\n\nProgram run:")
    interpret(tree)
    print()

# The following is a trick to make this testing code be ignored
# when this file is being imported, but run when run directly
# https://codefather.tech/blog/if-name-main-python/
if __name__ == '__main__':
    main()