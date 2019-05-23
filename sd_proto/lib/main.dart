import 'package:flutter/material.dart';
import 'code/dashboard_page.dart';
import 'code/analysis_page.dart';
import 'code/settings_page.dart';
import 'code/bluetooth_test_screen.dart';
import 'code/send_bluetooth_data.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'dart:async';
import 'package:firebase_auth/firebase_auth.dart';

void main(){
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  static BluetoothDevice device = null;
  static int testing = 33;
  static StreamSubscription deviceConnection = null;
  @override
  Widget build(BuildContext context){
    return MaterialApp(
      title: 'Testing Sign In with Firebase',
      home: new WelcomeScreen(),
      routes: <String, WidgetBuilder> {
        '/WelcomeScreen': (BuildContext context) => new WelcomeScreen(),
        '/DashboardScreen': (BuildContext context) => new DashboardScreen(),
        '/SettingsScreen' : (BuildContext context) => new SettingsScreen(),
        '/AnalysisScreen' : (BuildContext context) => new AnalysisScreen(),
        '/BluetoothTestScreen' : (BuildContext context) => new BluetoothTestScreen(),
        '/SendBluetoothData' : (BuildContext context) => new SendBluetoothData(),
      }
      );
  }
}

class WelcomeScreen extends StatelessWidget {

  final FirebaseAuth firebaseAuth = FirebaseAuth.instance;
  TextEditingController emailCtrl = new TextEditingController();
  TextEditingController passwordCtrl = new TextEditingController();

  Future<String> signIn(String email, String password) async {
    FirebaseUser user = await firebaseAuth.signInWithEmailAndPassword(
        email: email, password: password);
    print('I think we might have signed in...?');
    return user.uid;
  }

  @override
  Widget build(BuildContext context) {
    return new Scaffold(
        appBar: AppBar(
          title: const Text('Welcome To My App!'),
        ),
        body: Column(
            children: <Widget> [
              Container(
                child: TextField(
                  controller: emailCtrl,
                  autofocus: true,
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
              RaisedButton(
                child: const Text('Sign Up!'),
                onPressed: null,
              ),
              RaisedButton(
                  child: const Text('Sign In!'),
                  onPressed: () => signIn(emailCtrl.text, passwordCtrl.text)
              ),
            ]

        )
    );
  }
}



