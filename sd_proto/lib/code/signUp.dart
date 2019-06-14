import 'package:flutter/material.dart';
import 'dart:async';
import 'package:sd_proto/main.dart';

class SignUpScreen extends StatelessWidget {
  final TextEditingController emailCtrl = new TextEditingController();
  final TextEditingController passwordCtrl = new TextEditingController();
  final TextEditingController fnameCtrl = new TextEditingController();
  final TextEditingController lnameCtrl = new TextEditingController();
  final TextEditingController pinCtrl = new TextEditingController();

  Future<String> signUp(BuildContext context, String email, String password) async {
    MyApp.user = await MyApp.firebaseAuth.createUserWithEmailAndPassword(
        email: email, password: password);
    print('INFO: account created for ${MyApp.user.uid}');
    if(MyApp.user.uid != null) {
      Navigator.of(context).pushNamed('/DashboardScreen');
    }
    return MyApp.user.uid;
  }

  clearForm(BuildContext context) {
    //check for null b/c if form is blank and cancel button is clicked causes error
    if(emailCtrl.text != null){
      emailCtrl.clear();
    }
    if(passwordCtrl.text != null) {
      passwordCtrl.clear();
    }
    if(fnameCtrl.text != null) {
      fnameCtrl.clear();
    }
    if(lnameCtrl.text != null) {
      lnameCtrl.clear();
    }
    if(pinCtrl.text != null) {
      pinCtrl.clear();
    }
    Navigator.popUntil(context, ModalRoute.withName('/'));
  }

  @override
  Widget build(BuildContext context) {
    return new Scaffold(
        appBar: AppBar(
          title: const Text('Sign Up'),
        ),
        body: Column(
            children: <Widget> [
              Container(
                  child: Text('Please fill out the form below.'),
              ),
              Container(
                child: TextField(
                  controller: fnameCtrl,
                  autofocus: true,
                  decoration: new InputDecoration(
                    labelText: 'First Name'
                  )
                )
              ),
              Container(
                child: TextField(
                  controller: lnameCtrl,
                  autofocus: false,
                  decoration: new InputDecoration(
                    labelText: 'Last Name'
                  ),
                )
              ),
              Container(
                  child: TextField(
                    controller: emailCtrl,
                    autofocus: false,
                    decoration: new InputDecoration(
                        labelText: 'Email'
                    ),
                  )
              ),
              Container(
                  child: TextField(
                    controller: passwordCtrl,
                    autofocus: false,
                    decoration: new InputDecoration(
                        labelText: 'Password'
                    ),
                  )
              ),
              Container (
                child: TextField(
                  controller: pinCtrl,
                  autofocus: false,
                  decoration: new InputDecoration(
                    labelText: 'Pin Code'
                  ),
                )
              ),
              Row(
                children: <Widget>[

                  RaisedButton(
                    child: const Text('Submit'),
                    onPressed: () => signUp(context, emailCtrl.text, passwordCtrl.text),
                  ),
                  //add column for padding
                  RaisedButton(
                    child: const Text('Cancel'),
                    onPressed: () => clearForm(context),
                  ),
                ],
              )
            ]
        )
    );
  }
}