# from Cheetah.Template import Template
from PL0_H import PL0_H
from MAIN_C import MAIN_C


header = {"name": "PLO_H",
          "file_name": "plo.h"}

macros = [{"name": "NUM_MAX", "value": 10}]

enums = [{'name': 'symbol',
          'values': ['BEGIN', 'END', 'CALL']}]

structs = [{'name': 'word_etype_t',
            'items': [{'type': 'char', 'name': 'szWord[10]'},
                      {'type': 'enum symbol', 'name': 'eType'}]}]

variables = [{'type': 'FILE *', 'name': 'fPointer'}]

functions = [{'return_type': 'int', 'name': 'print_source_file()'}]

def header_file():
    
    print PL0_H(searchList=[{'header': header,
                             'macros': macros,
                             'enums': enums,
                             'structs': structs,
                             'variables': variables,
                             'functions': functions}])

def lexical():
    macros = {'compiled_file': 'first.pl0'}
    functions = {'init': 'init()',
                 'lexical': 'lexical_analysis()',
                 'print_source': 'print_source()'}
    variables = {'fP': 'fPointer'}

    # enummap = {'A': 'enumA',
    #            'B': 'enumB',
    #            'C': 'enumC',
    #            'D': 'enumD',
    #            'E': 'enumE',
    #            'F': 'enumF'}
    
    def emap(s):
        return 'enum' + s

    lower = [chr(i) for i in range(97, 123)]
    upper = [chr(i) for i in range(65, 91)]
    digit = [chr(i) for i in range(48, 58)]

    struct_dict = {'A': {tuple(lower + upper): 'B',
                         tuple(digit): 'C',
                         (':'): 'D',
                         ('=', '<', '>'): 'E',
                         ('+', '-', '*', '/', ';', ',', '.', '#', '(', ')'): 'Z',
                         (' ', '\t', '\n'): 'A'},
                  'B': {tuple(lower + upper + digit + ['_']): 'B',
                        (): 'Z'},
                  'C': {tuple(digit): 'C',
                        ('.'): 'F'},
                  'D': {('='): 'Z'},
                  'E': {('='): 'Z',
                        (): 'Z'},
                  'F': {tuple(digit): 'F',
                        (): 'Z'}}

    print MAIN_C(searchList = [{'struct_dict': struct_dict,
                                 'vars': variables,
                                 'funs': functions,
                                 'macros': macros,
                                'emap': emap}])
    

if __name__ == "__main__":
    #header_file()
    lexical()