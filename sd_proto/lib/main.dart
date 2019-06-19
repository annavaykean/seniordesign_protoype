import 'package:flutter/material.dart';
import 'code/dashboard_page.dart';
import 'code/settings_page.dart';
import 'code/signUp.dart';
import 'code/database_test_page.dart';
import 'code/stretches_page.dart';
import 'dart:async';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/services.dart';

void main(){
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  //global access to bluetooth connection
  static FirebaseAuth firebaseAuth = FirebaseAuth.instance;
  static FirebaseUser user;
  static StreamSubscription deviceConnection = null;
  static List postureDataList = null;
  static String pin;
  //global access to database connection
  static FirebaseDatabase database = new FirebaseDatabase();
  static DatabaseReference userReference;
  static DatabaseTestPageState databaseData = new DatabaseTestPageState();
  static String pose;
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
        '/DatabaseTestPage' : (BuildContext context) => new DatabaseTestPage(),
        '/Stretches' : (BuildContext context) => new Stretches(),
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
  String errorMessage = '';


  Future<String> signIn(BuildContext context, String email, String password) async {
    if(email != null && password != null) {
      try {
        MyApp.user = await MyApp.firebaseAuth.signInWithEmailAndPassword(
            email: email, password: password);
      } on PlatformException catch (e){
          print('INFO: ${e}');
          errorMessage = "Invalid login. Try again.";
      }
      if(MyApp.user != null) {
        print('INFO: ${MyApp.user.email} signed in.');
        //establish new database session

        //save data offline until connection is reestablished
        MyApp.database.setPersistenceEnabled(true);
        MyApp.database.setPersistenceCacheSizeBytes(10000000);
        //get reference to user's document within database
        MyApp.userReference = MyApp.database.reference().child('0000');
        //pull posture data
        MyApp.userReference.once().then((DataSnapshot snapshot) {
          List list = [];
          for(var value in snapshot.value.values) {
            //was previously experiencing errors on parsing from json. Fixed by converting data to string then to int.
            var cogX = value['cogX'].toString();
            var xInt = int.parse(cogX);
            var cogY = value['cogY'].toString();
            var yInt = int.parse(cogY);
            var created_at = value['created_at'].toString();
            //add parsed data to list as a Posture object
            list.add(new Posture(xInt, yInt, created_at));
          }
          //update global posture data list with fresh data.
          MyApp.postureDataList = list;
          });
        //navigate to homepage and dismiss keyboard
        FocusScope.of(context).requestFocus(new FocusNode());
        print('going to dashboard...');
        errorMessage = "";
        Navigator.of(context).pushReplacementNamed('/DashboardScreen');
        return MyApp.user.uid;
      }
    }
    else{
      errorMessage = 'Invalid login. Try again.';
      print('Must input email and password to sign in');
    }
  }

  Future<void> signOut() async {
    if(MyApp.firebaseAuth != null) {
      return MyApp.firebaseAuth.signOut();
    }
    if(MyApp.user != null) {
      MyApp.user = null;
    }
    Navigator.of(context).pushNamed('/WelcomeScreen');
    if(MyApp.deviceConnection != null) {
      MyApp.deviceConnection.cancel();
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
              Text(errorMessage),
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

class Posture {
  int cogX;
  int cogY;
  String created_at;
  Posture(this.cogX, this.cogY, this.created_at);

}



