import 'package:flutter/material.dart';
import 'code/dashboard_page.dart';
import 'code/settings_page.dart';
import 'code/signUp.dart';
import 'code/database_test_page.dart';
import 'code/stretches_page.dart';
import 'dart:async';
import 'dart:typed_data';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/services.dart';
import 'package:flutter_local_notifications/flutter_local_notifications.dart';
void main(){
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  //global access to firebase connection information and user settings

  static FirebaseAuth firebaseAuth = FirebaseAuth.instance;
  static FirebaseUser user;
  static StreamSubscription deviceConnection = null;
  static List postureDataList = null;
  static String pin;
  static String prefsPin = 'pinNum';
  //global access to database connection
  static FirebaseDatabase database = new FirebaseDatabase();
  static DatabaseReference userDataReference;
  static DatabaseReference userSettingsReference;
  static DatabaseTestPageState databaseData = new DatabaseTestPageState();
  static String pose;
  static bool vibration = true;
  static bool notification = true;
  static FlutterLocalNotificationsPlugin notificationsPlugin;
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

  initState() {
    super.initState();

     var initializationSettingsAndroid = new AndroidInitializationSettings('@mipmap/ic_launcher');
     var initializationSettingsIOS = new IOSInitializationSettings();
     var initializationSettings = new InitializationSettings(initializationSettingsAndroid, initializationSettingsIOS);

     MyApp.notificationsPlugin = new FlutterLocalNotificationsPlugin();
     MyApp.notificationsPlugin.initialize(initializationSettings, onSelectNotification: onSelectNotification);


  }

  Future<String> getPinNumber() async {
    final SharedPreferences prefs =  await SharedPreferences.getInstance();
    print('fetching pin number...');
    return prefs.getString(MyApp.prefsPin)?? 'invalid';
  }


  Future onSelectNotification(String payload) async {
    showDialog(
        context: context,
        builder: (_) => new AlertDialog(
          title: const Text('Here is your payload'),
          content: new Text('Payload: $payload'),
        )
    );
  }

   signIn(BuildContext context, String email, String password) async {
    if(email != null && password != null) {
      try {
        MyApp.user = await MyApp.firebaseAuth.signInWithEmailAndPassword(
            email: email, password: password);
      } on PlatformException catch (e){
          print('INFO: ${e}');
          setState(() {
            errorMessage = "Invalid login. Try again.";
          });
      }
      if(MyApp.user != null) {
        errorMessage = "";
        emailCtrl.clear();
        passwordCtrl.clear();
        print('INFO: ${MyApp.user.email} signed in.');
        var recievePin = await getPinNumber();
        print('recieved pin: $recievePin');
        if(recievePin != 'invalid') {
          MyApp.pin = recievePin;
          //establish new database session
          //save data offline until connection is reestablished
          MyApp.database.setPersistenceEnabled(true);
          MyApp.database.setPersistenceCacheSizeBytes(10000000);
          //get reference to user's document within database
          MyApp.userDataReference =
              MyApp.database.reference().child('postureData').child(recievePin);
          //pull posture data
          MyApp.userDataReference.once().then((DataSnapshot snapshot) {
            List list = [];
            for (var value in snapshot.value.values) {
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
          Navigator.of(context).pushReplacementNamed('/DashboardScreen');
        }
      }
    }
    else{
      errorMessage = 'Invalid login. Try again.';
      print('Must input email and password to sign in');
    }
  }

/*  Future<void> signOut() async {
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
  }*/

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
                  autofocus: false,
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



