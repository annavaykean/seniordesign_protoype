import 'package:flutter/material.dart';
import 'code/dashboard_page.dart';
import 'code/analysis_page.dart';
import 'code/settings_page.dart';
import 'code/bluetooth_test_screen.dart';
import 'code/send_bluetooth_data.dart';
import 'code/signUp.dart';
import 'code/database_test_page.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'dart:async';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:google_sign_in/google_sign_in.dart';
import 'package:flutter/services.dart';
void main(){
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  //global access to bluetooth connection
  static FirebaseAuth firebaseAuth = FirebaseAuth.instance;
  static GoogleSignIn gSignIn = new GoogleSignIn();
  static FirebaseUser user;
  static BluetoothDevice device = null;
  static int testing = 33;
  static StreamSubscription deviceConnection = null;
  static List<BluetoothService> services = new List();
  static List postureDataList = null;
  static String pin;
  //global access to database connection
  static FirebaseDatabase database = new FirebaseDatabase();
  static DatabaseReference userReference;

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
        '/DatabaseTestPage' : (BuildContext context) => new DatabaseTestPage(),
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
      try {
        MyApp.user = await MyApp.firebaseAuth.signInWithEmailAndPassword(
            email: email, password: password);
      } on PlatformException catch (e){
          print('INFO: ${e}');
      }
      if(MyApp.user != null) {
        print('INFO: ${MyApp.user.email} signed in.');
        //establish new database session

        //save data offline until connection is reestablished
        MyApp.database.setPersistenceEnabled(true);
        MyApp.database.setPersistenceCacheSizeBytes(10000000);
        //get reference to user's document within database
       // MyApp.userReference = MyApp.database.reference().child('user)');
        MyApp.userReference = MyApp.database.reference().child('0000');
        //navigate to homepage
        Navigator.of(context).pushReplacementNamed('/DashboardScreen');
        return MyApp.user.uid;
      }
    }
    else{
      print('Must input email and password to sign in');
    }
  }

  Future<void> signOut() async {
    if(MyApp.gSignIn != null) {
      return MyApp.gSignIn.signOut();
    }
    else {
      return MyApp.firebaseAuth.signOut();
    }
  }

  @override
  Widget build(BuildContext context) {
    return new Scaffold(
        appBar: AppBar(
          title: const Text('Welcome to Smart Chair!'),
        ),
        body: Center(
        child: Column(
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
                  obscureText: true,
                  decoration: new InputDecoration(
                    labelText: 'Password',
                    errorText: validPass ? 'Required Field' : null,
                  ),
                )
              ),
              ButtonTheme(
                minWidth: 300.0,
                child:  RaisedButton(
                    child: const Text('Sign In'),
                    color: Colors.blue,
                    textColor: Colors.white,
                    splashColor: Colors.purple,
                    onPressed: () {
                      setState((){
                        emailCtrl.text.isEmpty ? validEmail = true : validEmail = false;
                        passwordCtrl.text.isEmpty ? validPass = true : validPass = false;
                      });
                      signIn(context, emailCtrl.text, passwordCtrl.text);
                    }
                ),
              ),
              FlatButton(
                child: const Text('Create an account'),
                splashColor: Colors.blue,
                onPressed: (){
                  Navigator.of(context).pushNamed('/SignUp'); },
              ),
            ]

        ))
    );
  }
}



