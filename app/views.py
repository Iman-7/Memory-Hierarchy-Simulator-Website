from app import app
import subprocess
from flask import render_template, request, redirect
from datetime import datetime
import os
from flask import Flask, flash, request, redirect, url_for
from werkzeug.utils import secure_filename

@app.template_filter("clean_date")
def clean_date(dt):
    return dt.strftime("%d %b %Y")

@app.route("/")
def home():
    return render_template("public/index.html")

@app.route("/about")
def about():
    return render_template("public/about.html")


UPLOAD_FOLDER = 'D:\\C++ projects\\Project2_Assembly\\app\\static\\uploaded'
ALLOWED_EXTENSIONS = {'txt'}

# app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

def allowed_file(filename):
    return '.' in filename and \
           filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

@app.route('/test/<S>/<L>/<cycles>', methods=['GET', 'POST'])
def upload_file(S, L, cycles):
    print ("Yesss upload!!", S, L, cycles)

    if request.method == 'POST':
        # check if the post request has the file part
        if 'file' not in request.files:
            flash('No file part')
            return redirect(request.url)
        file = request.files['file']
        # If the user does not select a file, the browser submits an
        # empty file without a filename.
        if file.filename == '':
            flash('No selected file')
            return redirect(request.url)
        if file and allowed_file(file.filename):
            file_name = os.path.join(app.config['UPLOAD_FOLDER'], secure_filename(file.filename))
            file.save(file_name)
            return redirect(url_for('output', name=file_name, S=S, L=L, cycles = cycles))
    return render_template("public/yayyy.html")


@app.route('/output/<name>/<S>/<L>/<cycles>')  
def output(name, S, L, cycles):
    command = "{} {} {} ".format(S,L,cycles)
    print("wareeeniiii bt3ml ehhh", command)
    command ='.\main {} "{}"'.format(command, name)
    print("hiiiiii wareenii ", command)
    out = subprocess.check_output(command, shell = True).decode("utf-8")
    print(out)
    C = int(S)/int(L)
    print("C =", C)
    return render_template("public/output.html", out=out, C = C)

@app.route('/form')
def my_form():
    return render_template("public/form.html")
@app.route('/form', methods=['POST'])
def my_form_post():
    text = request.form['box1']
    S = text.upper()
    text = request.form['box2']
    L = text.upper()
    text = request.form['box3']
    cycles = text.upper()
    print("Form doneee")
    return redirect(url_for('upload_file', S=S, L= L, cycles = cycles))
