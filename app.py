#!/usr/bin/env python
# -*- coding: ISO-8859-1 -*-

import os
import pylibfromCFFI

from flask import Flask
from flask import request
from flask import render_template
#from flask_paginate import Pagination, get_page_parameter

THIS_FOLDER = os.path.dirname(os.path.abspath(__file__))

app 		= Flask(__name__)
app.debug 	= False

@app.route('/minigoogle', methods=['POST']) 
def minigoogle():
	global THIS_FOLDER

	query = request.form.get('query')
	print("query: ",query)

	word = query


	#MYFILE = 'core/comm/log.txt'
	#FILENAME = os.path.join(THIS_FOLDER, MYFILE)
	#with open(FILENAME, 'wb') as x_files:
	#	query_bin = query.encode('utf-8')
	#	x_files.write(query_bin)
	#	x_files.close()

	numberResult = pylibfromCFFI.get_results(query.encode('ISO-8859-1'))

	APPFILES = 'core/sug.txt'
	FILENAME = os.path.join(THIS_FOLDER, APPFILES)
	with open(FILENAME, 'r', encoding = 'ISO-8859-1') as r_file_sug:
		sug = []
		i = 1
		sug = r_file_sug.readlines()
		r_file_sug.close()


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

	totalPage = int(numberResult/20)+1
	actualPage = 0

	sugerencia = ""
	lensugerencia = len(sug) 
	for ai in range(len(sug)):
		sugerencia = sug[ai]

	return render_template('index.html', ti=listDbIndex, numberResult = numberResult, totalPage=totalPage, actualPage=actualPage, word=word, sugerencia=sugerencia, lensugerencia=lensugerencia)

@app.route('/minigoogle/<int:numberResult>/<int:totalPage>/<int:actualPage>/<string:word>') 
def minigooglePage(numberResult, totalPage, actualPage, word):
	global THIS_FOLDER

	APPFILE = 'core/toapp.txt'
	FILENAME = os.path.join(THIS_FOLDER, APPFILE)
	with open(FILENAME, 'r', encoding = 'ISO-8859-1') as r_files:
		content = []
		i = 1
		content = r_files.readlines()[(20*(actualPage)):20*(actualPage+1)]
		
		r_files.close()

	listDbIndex = []

	for ai in range(len(content)):

		for j in range(len(content[ai])):
			if content[ai][j:j+1] == '|':
				listDbIndex.append([content[ai][0:j],content[ai][j+1:]])
				break

	lensugerencia = 0

	return render_template('index.html', ti=listDbIndex, numberResult = numberResult, totalPage=totalPage, actualPage=actualPage, word=word, lensugerencia=lensugerencia)

@app.route('/view/<int:dbindex>/<string:titulo>')
def view(dbindex, titulo):
	#print("--->",dbindex, "-", titulo)
	#print("<<>><<>>",pylibfromCFFI.get_contenido('-'.encode('ISO-8859-1'), dbindex))
	contenido = pylibfromCFFI.get_contenido('-'.encode('ISO-8859-1'), dbindex)
	return render_template('viewarticle.html', titulo=titulo, contenido = contenido)


@app.route('/')
def home():
	totalPage = 0
	word =""
	lensugerencia = 0
	return render_template('index.html', totalPage= totalPage, lensugerencia = lensugerencia)

if __name__ == '__main__':
	pylibfromCFFI.make_suffix_tree()
	app.run(host='0.0.0.0', port=5000, debug=False)
