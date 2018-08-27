import os
from flask import Flask
from flask import request
from flask import render_template

THIS_FOLDER = os.path.dirname(os.path.abspath(__file__))

app 		= Flask(__name__)
app.debug 	= True

@app.route('/foo', methods=['POST']) 
def foo():
	global THIS_FOLDER

	query = request.form.get('query')
	print("query: ",query)


	MYFILE = 'core/comm/log.txt'
	FILENAME = os.path.join(THIS_FOLDER, MYFILE)
	with open(FILENAME, 'wb') as x_files:
		query_bin = query.encode('utf-8')
		#x_files.write('{}'.format(query))
		x_files.write(query_bin)
		x_files.close()
	return render_template('index.html')

@app.route('/')
def home():
	return render_template('index.html')

if __name__ == '__main__':
	app.run(host='0.0.0.0', port=5000, debug=True)