# Illya Gordyy
# igordyy@csus.edu


class scanner:
    # toks[i] must evaluate to the i-th token in the token stream.
    # Assumes toks does not change during parsing
    def __init__(self,toks):
        self._toks = toks
        self._i = 0
    
    # If no more tokens exist or current token isn't s, raise exception.
    # Otherwise pass over the current one and move to the next.
    def match(self,s):
        if (self._i < len(self._toks)) and (self._toks[self._i] == s):
            self._i += 1
        else:
            raise Exception
            
    # If any tokens remain return the current one. If no more, return None.
    def next(self):
        if self._i < len(self._toks):
            return self._toks[self._i]
        else:
            return None


# Input can be any type where len(input) is defined and input[i] yields a
# string (ie, string, list, etc). Raises Exception on a parse error.
# S → <AB
# A → aAb | b
# B → bB | >
def parse1(input):
    toks = scanner(input)
    stack = ['S']
    while len(stack) > 0:
        top = stack.pop()           # Always pop top of stack
        tok = toks.next()           # None indicates token stream empty
        if top in ('<','a','b','>'):  # Matching stack top to token
            toks.match(top)
        elif top == 'S' and tok == '<': 
            stack.append('B')            
            stack.append('A')
            stack.append('<')
        elif top == 'A' and tok == 'a': 
            stack.append('b')         
            stack.append('A')
            stack.append('a')
        elif top == 'A' and tok == 'b': 
            stack.append('b')           
        elif top == 'B'and tok == 'b':
            stack.append('B')
            stack.append('b')
        elif top == 'B'and tok == '>':
            stack.append('>')
        else:
            raise Exception    # Unrecognized top/tok combination
    if toks.next() != None:
        raise Exception


# Input can be any type where len(input) is defined and input[i] yields a
# string (ie, string, list, etc). Raises Exception on a parse error.
# S → BA
# A → +BA | -BA | λ
# B → DC
# C → *DC | /DC | λ
# D → a | (S)

    
def parse2(input):
    toks = scanner(input)
    stack = ['S']
    while len(stack) > 0:
        top = stack.pop()      # Always pop top of stack
        tok = toks.next()      # None indicates token stream empty
        if top in ('a','+','-','*','/','(',')'):     # Matching stack top to token
            toks.match(top)
        elif top == 'S' and tok in ('a', '('): 
            stack.append('A')
            stack.append('B')
        elif top == 'A' and tok == '+':  
            stack.append('A')           
            stack.append('B')
            stack.append('+')
        elif top == 'A' and tok == '-':
            stack.append('A')           
            stack.append('B')
            stack.append('-')
        elif top == 'A' and tok in (')', None):
            pass
        elif top == 'B' and tok in ('a','('):  
            stack.append('C')            
            stack.append('D')
        elif top == 'C' and tok == '*':  
            stack.append('C')    
            stack.append('D')    
            stack.append('*')
        elif top == 'C' and tok == '/':  
            stack.append('C')    
            stack.append('D')    
            stack.append('/')         
        elif top == 'C' and tok in ('+','-',')',None):  
            pass
        elif top == 'D' and tok == 'a':  
            stack.append('a')
        elif top == 'D' and tok == '(':
            stack.append(')')
            stack.append('S')
            stack.append('(')
        else:
            raise Exception    # Unrecognized top/tok combination
    if toks.next() != None:
        raise Exception


# The following is a trick to make this testing code be ignored
# when this file is being imported, but run when run directly
# https://codefather.tech/blog/if-name-main-python/
if __name__ == '__main__':
    try:
        parse1("<abb>")
    except:
        print("Reject")
    else:
        print("Accept")

    try:
        parse2("a-a/a+(a+a)")
    except:
        print("Reject")
    else:
        print("Accept")
    
    try:
        parse2("")
    except:
        print("Reject")
    else:
        print("Accept")


