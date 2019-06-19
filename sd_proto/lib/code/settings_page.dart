import 'package:flutter/material.dart';
import 'package:sd_proto/main.dart';

class SettingsScreen extends StatefulWidget{
  @override
  SettingsScreenState createState() => SettingsScreenState();
}
class SettingsScreenState extends State<SettingsScreen>{
  bool vibrationToggle = true;
  bool notificationToggle = true;

  updateVibration() {
    if (vibrationToggle) {
      setState(() {
        vibrationToggle = false;
      });
    } else {
      setState(() {
        vibrationToggle = true;
      });
    }
    //update flag in firebase
  }

  updateNotifications () {
    if(notificationToggle) {
      setState(() {
        notificationToggle = false;
      });
    } else {
      setState(() {
        notificationToggle = true;
      });
    }
  }
  signOut(BuildContext context) {
    if(MyApp.firebaseAuth != null) {
      MyApp.firebaseAuth.signOut();
    }
    if(MyApp.user != null) {
      MyApp.user = null;
    }
    Navigator.of(context).pushNamed('/WelcomeScreen');
    if(MyApp.deviceConnection != null) {
      MyApp.deviceConnection.cancel();
    }

  }

  Widget listViewConstructor (BuildContext context) {
    return ListView(
      children: <Widget>[
        ListTile(
          title: Text('Toggle Vibration'),
          trailing: Switch(value: vibrationToggle, onChanged: (value) => updateVibration())
        ),
        ListTile(
          title: Text('Toggle Phone Notifications'),
         trailing: Switch(value: notificationToggle, onChanged: (value) => updateNotifications()),
        ),
        ListTile(
          title: Text('Sign Out'),
         onTap: () => signOut(context),
        )
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    return new Scaffold(
        appBar: AppBar(
          title: const Text('This will be the settings page!'),
        ),
        body: listViewConstructor(context),
    );
  }
}