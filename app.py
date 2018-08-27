from flask import Flask
from flask import request
from flask import render_template

app 			= Flask(__name__)
app.debug 	= True

@app.route('/foo', methods=['POST']) 
def foo():
	query = request.form.get('query')
	print("query: ",query)

	path = 'core/comm/log.txt'
	with open(path, 'wb') as x_files:
		x_files.write('{}'.format(query))
	return render_template('index.html')

@app.route('/')
def home():
	return render_template('index.html')

if __name__ == '__main__':
	app.run(host='0.0.0.0', port=5000, debug=True)