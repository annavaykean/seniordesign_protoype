import 'package:flutter/material.dart';
import 'dart:async';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:sd_proto/main.dart';
import 'package:intl/intl.dart';
import 'package:flutter/services.dart';
import 'package:firebase_database/firebase_database.dart';

class SignUpScreen extends StatefulWidget {
  @override
  SignUpScreenState createState() => SignUpScreenState();
}
class SignUpScreenState extends State<SignUpScreen> {
  final TextEditingController emailCtrl = new TextEditingController();
  final TextEditingController passwordCtrl = new TextEditingController();
  final TextEditingController pinCtrl = new TextEditingController();
  String errorMssg = '';
  String pinErrorMssg = '';
  bool pinIsValid = false;

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
        "firePhoneNotif" : "0",
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
          String formattedDate = DateFormat('yyyyMMddTkkmms').format(now);
          //01012001_00010
          //YYYYMMDDTHHMMSS
          String created = '20010101T00000' + counter.toString();
          counter++;
       //  Timer timer = new Timer(const Duration(milliseconds: 1000), (){
            MyApp.userDataReference.child(created).set(<String, String>{
              "cogX": "" + initCogX[i],
              "cogY": "" + initCogY[i],
              "created_at": "" + created,
            }).then((result) {
              print(
                  "INFO: Database Write Completed (${initCogX[i]}, ${initCogY[i]})");
            });
   // });
        }
      }
      //dismiss keyboard
  }
  verifyPin(String pinCode) {
    String pin = pinCode.trim();
    //double check pin is not already in use before creating account
    var dbQuery = MyApp.database.reference().child('usedPins').child(pin);
    dbQuery.once().then((DataSnapshot snapshot) {
      if (snapshot.value != null) {
        print('Pin already in use');
        pinErrorMssg = 'This pin is unavailable.';
        pinIsValid = false;
        setState(() {});
      } else {
        MyApp.database.reference().child('usedPins').set(<String, String>{
          "" + pin : "1",
        }).then((result) {
          print(
              "INFO: Database Write Completed (pin $pin has been reserved)");
        });
        print('Pin is now reserved.');
        pinErrorMssg = '';
        pinIsValid = true;
        setState(() {});
      }
    });
  }

  Future<String> signUp(BuildContext context, String email, String password, String pin) async {
    String emailTrimmed = email.trim();
    String passwordTrimmed = password.trim();
    //create user account
    if(pinIsValid){
    try {
      MyApp.user = await MyApp.firebaseAuth.createUserWithEmailAndPassword(
          email: emailTrimmed, password: passwordTrimmed);
      errorMssg = '';
      setState(() {});
      //get pin from form to setup database
      MyApp.pin = pin;
      //save pin number as shared preferences
      setPinNumber();
      print('INFO: account created for ${MyApp.user.uid}');
      //send initial data points to firebase (the 4 points to control chart shape) and preferences
      writeInitPointsToDatabase();
      setupUserPreferences();

      //go to dashboard
      if (MyApp.user.uid != null) {
        Navigator.pushReplacementNamed(context, '/WelcomeScreen');
      }
      return MyApp.user.uid;
      }
      on PlatformException catch (e) {
        print(e);
        errorMssg = e.message;
        setState(() {});
      }

    }
    else {
      print('Must verify pin before account creation');
      pinErrorMssg = "Must verify pin before creating account!";
      setState(() {});
    }

  }

  clearForm(BuildContext context) {
    //check for null b/c if form is blank and cancel button is clicked causes error
    if(emailCtrl.text != null){
      emailCtrl.clear();
    }
    if(passwordCtrl.text != null) {
      passwordCtrl.clear();
    }
    if(pinCtrl.text != null) {
      pinCtrl.clear();
    }
    Navigator.pushReplacementNamed(context, '/WelcomeScreen');
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
              Row(
              children: <Widget> [
                Container (
                    width: 100.0,
                    child: TextField(
                      controller: pinCtrl,
                      autofocus: false,
                      decoration: new InputDecoration(
                          labelText: 'Pin Code'
                      ),
                    )
                ),
              RaisedButton(
                child: pinIsValid ? Text('Pin Verified!') : Text('Verify Pin'),
                color: pinIsValid ? Colors.green : null,
                onPressed: () => verifyPin(pinCtrl.text),
              ),
              ]),
              Container(
                child: Text(errorMssg,
                       style: TextStyle(color: Colors.red)),
              ),
              Container(
                child: Text(pinErrorMssg,
                       style: TextStyle(color: Colors.red)),
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