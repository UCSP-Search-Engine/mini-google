#!/usr/bin/env python
# -*- coding: ISO-8859-1 -*-

import os
import pylibfromCFFI

from numpy import *
from flask import Flask
from flask import request
from flask import render_template
from flask_paginate import Pagination, get_page_parameter

THIS_FOLDER = os.path.dirname(os.path.abspath(__file__))

app 		= Flask(__name__)
app.debug 	= False

@app.route('/minigoogle', methods=['POST']) 
def minigoogle():
	global THIS_FOLDER

	query = request.form.get('query')
	print("query: ",query)


	#MYFILE = 'core/comm/log.txt'
	#FILENAME = os.path.join(THIS_FOLDER, MYFILE)
	#with open(FILENAME, 'wb') as x_files:
	#	query_bin = query.encode('utf-8')
	#	x_files.write(query_bin)
	#	x_files.close()

	pylibfromCFFI.get_results(query.encode('ISO-8859-1'))

	APPFILE = 'core/toapp.txt'
	FILENAME = os.path.join(THIS_FOLDER, APPFILE)
	with open(FILENAME, 'r', encoding = 'ISO-8859-1') as r_files:
		content = []
		i = 1
		#while (len(content)==0):
			#extraer = r_files.readlines()
			#for ex in extraer:
			#	if ex == '|':
			#		print('N')
		content = r_files.readlines()[0:20]
		numberResult = len(r_files.readlines())
		#print(r_files.readlines()[1:2])
		r_files.close()

	listDbIndex = []

	# Append empty lists in first two indexes.

	i = 0

	for ai in range(len(content)):
		#listDbIndex[ai].append(content[ai][0:5])
		#listDbIndex[ai].append(content[ai][5:])
		
		#listDbIndex.append([content[ai][0:5],content[ai][0:5]])
		for j in range(len(content[ai])):
			if content[ai][j:j+1] == '|':
				listDbIndex.append([content[ai][0:j],content[ai][j+1:]])
				break
				#print (j)

		#for j in range(len(content[ai])):
		#	print("->",content[ai][0:5])
		#	filterContent[j][0] = content[ai]
		#	filterContent[j][1] = content[ai][1:3]

	#print(listDbIndex)

	for ci in range(len(content)):
		cont 		= content[ci]
		cont_list 	= list(cont)
		for i in range(len(cont_list)):
			if (ord(cont_list[i]) == 241):
				cont_list[i] = 'ni'
				#cont_list[i] = 'ñ'
				#print(ord(c))
		cont = "".join(cont_list)
		content[ci] = cont

	for cont in content:
		print (cont)
	data = {}
	data['titles'] = content

	longitud = len(content)

	return render_template('index.html', result=data, ti=listDbIndex, cantidad=longitud, numberResult = numberResult)

@app.route('/view/<int:dbindex>/<string:titulo>')
def view(dbindex, titulo):
	#print("--->",dbindex, "-", titulo)
	print("<<>><<>>",pylibfromCFFI.get_contenido('-'.encode('ISO-8859-1'), dbindex))
	contenido = pylibfromCFFI.get_contenido('-'.encode('ISO-8859-1'), dbindex)
	return render_template('viewarticle.html', titulo=titulo, contenido = contenido)


@app.route('/')
def home():
	return render_template('index.html')

if __name__ == '__main__':
	pylibfromCFFI.make_suffix_tree()
	app.run(host='0.0.0.0', port=5000, debug=False)
