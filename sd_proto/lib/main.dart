import 'package:flutter/material.dart';
import 'code/dashboard_page.dart';
import 'code/settings_page.dart';
import 'code/signUp.dart';
//import 'code/database_test_page.dart';
import 'code/stretches_page.dart';
import 'code/longTermData.dart';
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
  //static DatabaseTestPageState databaseData = new DatabaseTestPageState();
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
       // '/DatabaseTestPage' : (BuildContext context) => new DatabaseTestPage(),
        '/Stretches' : (BuildContext context) => new Stretches(),
        '/LongTermData': (BuildContext context) => new LongTermData(),
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
    // super.initState();

    var initializationSettingsAndroid = new AndroidInitializationSettings('@mipmap/ic_launcher');
    var initializationSettingsIOS = new IOSInitializationSettings();
    var initializationSettings = new InitializationSettings(initializationSettingsAndroid, initializationSettingsIOS);

    MyApp.notificationsPlugin = new FlutterLocalNotificationsPlugin();
    MyApp.notificationsPlugin.initialize(initializationSettings, onSelectNotification: onSelectNotification);

    //timer for checking database to determine if notification should be sent
    //   print('priming timer!');
    //check notification settings every 30 seconds
    const timeCheck = const Duration(seconds: 30);
    var timer = new Timer.periodic(timeCheck, (timer) {
      //    print('timer ran out!');
      //check firebase to see if notif should be fired
      if(MyApp.user != null){
        //   print('checking if notification is needed');
        var notifQuery = MyApp.database.reference().child('settings').child(
            MyApp.pin);
        notifQuery.once().then((DataSnapshot snapshot) {
          if (snapshot.value != null) {
            if(snapshot.value['firePhoneNotif'] != null && snapshot.value['firePhoneNotif'] != '0') {
              print('firing notification!');
              sendNotification(snapshot.value['firePhoneNotif']);
              //reset flag in firebase
              var resetDB = MyApp.database.reference().child('settings')
                  .child(MyApp.pin)
                  .update(
                  <String, String>{
                    "firePhoneNotif" : "0",
                  })
                  .then((result) {
                print("INFO: Database Write Completed");
              });
           //   initState();
            }
          } /*else {
            initState();
          }*/
        });
      }
    });

    //remind user to get up every 90 mins
    const getUpCheck = const Duration(minutes: 5);
    var timer2 = new Timer.periodic(getUpCheck, (timer2) {
      //    print('timer ran out!');
      //check firebase to see if notif should be fired
      if(MyApp.user != null){
        //   print('checking if notification is needed');
        var notifQuery = MyApp.database.reference().child('settings').child(
            MyApp.pin);
        notifQuery.once().then((DataSnapshot snapshot) {
          if (snapshot.value != null) {
            if(snapshot.value['getUp'] == '1') {
              print('firing notification!');
              sendNotification("Take a break from sitting!");
              //reset flag in firebase
              var resetDB = MyApp.database.reference().child('settings')
                  .child(MyApp.pin)
                  .update(
                  <String, String>{
                    "getUp" : "0"
                  })
                  .then((result) {
                print("INFO: Database Write Completed");
              });
              initState();
            }
          } else {
            initState();
          }
        });
      }
    });

  }

  Future sendNotification(String feedback) async {
    if (MyApp.notification) {
      var vibrationPattern = Int64List(4);
      vibrationPattern[0] = 0;
      vibrationPattern[1] = 1000;
      vibrationPattern[2] = 5000;
      vibrationPattern[3] = 2000;
      var androidPlatformChannelSpecifics = new AndroidNotificationDetails(
          'notofication_channel_id', 'Channel Name',
          'Here we will put the description about the Channel ',
          vibrationPattern: vibrationPattern,
          importance: Importance.Max, priority: Priority.High);

      var iOSPlatformChannelSpecifics = new IOSNotificationDetails();

      var platformChannelSpecifics = new NotificationDetails(
          androidPlatformChannelSpecifics, iOSPlatformChannelSpecifics);

      await MyApp.notificationsPlugin.show(
          0, 'Smart Chair', feedback,
          platformChannelSpecifics, payload: 'Default_Sound');
    } else {
      print('Notifications turned off.');
    }
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
    //remove white space
    String emailTrimmed = email.trim();
    String passwordTrimmed = password.trim();

    //check that form is valid
    if(email != null && password != null) {
      try {
        //attempt to sign in
        MyApp.user = await MyApp.firebaseAuth.signInWithEmailAndPassword(
            email: emailTrimmed, password: passwordTrimmed);
      } on PlatformException catch (e){
        //if sign in fails, display error message
          print('INFO: ${e}');
          setState(() {
            errorMessage = e.message;
          });
      }
      //if login is successful, execute if statement
      if(MyApp.user != null) {
        errorMessage = "";
        emailCtrl.clear();
        passwordCtrl.clear();
        print('INFO: ${MyApp.user.email} signed in.');

        //get pin from sharedPreferences
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
              print("Adding (" + xInt.toString() + ", " + yInt.toString() + ", " + created_at.toString() + ")\n");
              list.add(new Posture(xInt, yInt, created_at));
            }
            //update global posture data list with fresh data.
            MyApp.postureDataList = list;
          });
          //navigate to homepage and dismiss keyboard
          FocusScope.of(context).requestFocus(new FocusNode());
          print('going to dashboard...');
          Navigator.pushReplacementNamed(context,'/DashboardScreen');
        }
      }
    }
    else{
      errorMessage = 'Invalid login. Try again.';
      print('Must input email and password to sign in');
    }
  }

  goToSignUp (BuildContext context) {
    Navigator.pushReplacementNamed(context, '/SignUp');
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
              Text(errorMessage,
                  style: TextStyle(color: Colors.red)),
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
                onPressed: () => goToSignUp(context),
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
