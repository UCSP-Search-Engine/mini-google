import cffi

ffi = cffi.FFI()
ffi.cdef("void cffi_get_results(char *);")
ffi.cdef("void cffi_make_suffix_tree();")
ffi.cdef("char* cffi_get_contenido(char*, int);")
C = ffi.dlopen("./milibreria.so")

def get_results(word):
	#C.cffi_get_results(ffi.new('char[]',word.encode('utf-8'))
	C.cffi_get_results(word)

def make_suffix_tree():
	C.cffi_make_suffix_tree()

def get_contenido(contenido, dbIndex):
	return ffi.string(C.cffi_get_contenido(ffi.new('char[]',contenido),dbIndex)).decode('ISO-8859-1')