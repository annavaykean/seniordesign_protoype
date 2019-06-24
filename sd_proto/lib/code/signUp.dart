import 'package:flutter/material.dart';
import 'dart:async';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:sd_proto/main.dart';
import 'package:intl/intl.dart';

class SignUpScreen extends StatelessWidget {
  final TextEditingController emailCtrl = new TextEditingController();
  final TextEditingController passwordCtrl = new TextEditingController();
  final TextEditingController fnameCtrl = new TextEditingController();
  final TextEditingController lnameCtrl = new TextEditingController();
  final TextEditingController pinCtrl = new TextEditingController();


  Future<bool> setPinNumber() async {
    final SharedPreferences prefs = await SharedPreferences.getInstance();
    print('setting pin number');
    return prefs.setString(MyApp.prefsPin, MyApp.pin);
  }
  setupUserPreferences(){
    //initialize vibrate and notification preferences to ON
    if(MyApp.user != null) {
      var dbRef = MyApp.database.reference().child('settings').child(MyApp.pin);
      dbRef.set(<String, String>{
        "notification": "1",
        "vibration": "1",
      });
      //update global vars
      MyApp.notification = true;
      MyApp.vibration = true;
    }
  }
  writeInitPointsToDatabase() {
    //writes inital 4 values to firebase database to control shape of chart (so that(0,0) is center)
    //order of coords: FL (Quadrant 2), FR (Q1), RL (Q3), RR (Q4)
      List initCogX = ['-100', '100', '-100', '100'];
      List initCogY = ['100', '100', '-100', '-100'];

      if(MyApp.user != null){
        MyApp.userDataReference = MyApp.database.reference().child('postureData').child(MyApp.pin);
        int counter = 0;
        for(int i=0;i<4;i++) {
          DateTime now = DateTime.now();
          String created = '01012001_00010' + counter.toString();
          counter++;
       //  Timer timer = new Timer(const Duration(milliseconds: 1000), (){
            MyApp.userDataReference.child(created).set(<String, String>{
              "cogX": "" + initCogX[i],
              "cogY": "" + initCogY[i],
              "created_at": "" + now.toString(),
            }).then((result) {
              print(
                  "INFO: Database Write Completed (${initCogX[i]}, ${initCogY[i]})");
            });
   // });
        }
      }
      //dismiss keyboard
  }

  Future<String> signUp(BuildContext context, String email, String password, String pin) async {
    String emailTrimmed = email.trim();
    //create user account
    MyApp.user = await MyApp.firebaseAuth.createUserWithEmailAndPassword(
        email: emailTrimmed, password: password);
    //get pin from form to setup database
    MyApp.pin = pin;
    //save pin number as shared preferences
    setPinNumber();
    print('INFO: account created for ${MyApp.user.uid}');
    //send initial data points to firebase (the 4 points to control chart shape) and preferences
    writeInitPointsToDatabase();
    setupUserPreferences();

    //go to dashboard
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
                    obscureText: true,
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
                    onPressed: () => signUp(context, emailCtrl.text, passwordCtrl.text, pinCtrl.text),
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