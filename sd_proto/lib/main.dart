import 'package:flutter/material.dart';
import 'code/dashboard_page.dart';
import 'code/analysis_page.dart';
import 'code/settings_page.dart';
import 'code/bluetooth_test_screen.dart';
import 'code/send_bluetooth_data.dart';
import 'code/signUp.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'dart:async';
import 'package:firebase_auth/firebase_auth.dart';

void main(){
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  static FirebaseAuth firebaseAuth = FirebaseAuth.instance;
  static FirebaseUser user;
  static BluetoothDevice device = null;
  static int testing = 33;
  static StreamSubscription deviceConnection = null;
  static List<BluetoothService> services = new List();

  @override
  Widget build(BuildContext context){
    return MaterialApp(
      title: 'Testing Sign In with Firebase',
      home: new WelcomeScreen(),
      routes: <String, WidgetBuilder> {
        '/WelcomeScreen': (BuildContext context) => new WelcomeScreen(),
        '/SignUp': (BuildContext context) => new SignUpScreen(),
        '/DashboardScreen': (BuildContext context) => new DashboardScreen(),
        '/SettingsScreen' : (BuildContext context) => new SettingsScreen(),
        '/AnalysisScreen' : (BuildContext context) => new AnalysisScreen(),
        '/BluetoothTestScreen' : (BuildContext context) => new BluetoothTestScreen(),
        '/SendBluetoothData' : (BuildContext context) => new SendBluetoothData(),
      }
      );
  }
}
class WelcomeScreen extends StatefulWidget{
  @override
  WelcomeScreenState createState() => WelcomeScreenState();
}
class WelcomeScreenState extends State<WelcomeScreen> {

  final TextEditingController emailCtrl = new TextEditingController();
  final TextEditingController passwordCtrl = new TextEditingController();
  bool validEmail = false;
  bool validPass = false;
  Future<String> signIn(BuildContext context, String email, String password) async {
    if(email != null && password != null) {
      MyApp.user = await MyApp.firebaseAuth.signInWithEmailAndPassword(
          email: email, password: password);
      print('INFO: ${MyApp.user.uid} signed in.');
      Navigator.of(context).pushNamed('/DashboardScreen');
      return MyApp.user.uid;
    }
    else{
      print('Must input email and password to sign in');
    }
  }

  Future<void> signOut() async {
    return MyApp.firebaseAuth.signOut();
  }

  @override
  Widget build(BuildContext context) {
    return new Scaffold(
        appBar: AppBar(
          title: const Text('Welcome To My App!'),
        ),
        body: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          crossAxisAlignment: CrossAxisAlignment.center,
            children: <Widget> [
              Container(
                width: 300,
                child: TextField(
                  controller: emailCtrl,
                  autofocus: true,
                  decoration: new InputDecoration(
                    labelText: 'Email',
                    errorText: validEmail ? 'Required Field' : null,
                  ),
                )
              ),
              Container(
                width: 300,
                child: TextField(
                  controller: passwordCtrl,
                  autofocus: false,
                  decoration: new InputDecoration(
                    labelText: 'Password',
                    errorText: validPass ? 'Required Field' : null,
                  ),
                )
              ),
              RaisedButton(
                child: const Text('Sign Up'),
                onPressed: (){
                  Navigator.of(context).pushNamed('/SignUp'); },
              ),
              RaisedButton(
                  child: const Text('Sign In'),
                  onPressed: () {
                    setState((){
                      emailCtrl.text.isEmpty ? validEmail = true : validEmail = false;
                      passwordCtrl.text.isEmpty ? validPass = true : validPass = false;
                    });
                    signIn(context, emailCtrl.text, passwordCtrl.text);
                  }
              ),
            ]

        )
    );
  }
}



