import 'package:flutter/material.dart';
import 'package:sd_proto/main.dart';
import 'dart:typed_data';
import 'package:flutter_local_notifications/flutter_local_notifications.dart';
import 'package:swipedetector/swipedetector.dart';
class SettingsScreen extends StatefulWidget{
  @override
  SettingsScreenState createState() => SettingsScreenState();
}
class SettingsScreenState extends State<SettingsScreen> {
  //get preferences from firebase
  bool vibrationToggle = MyApp.vibration;
  bool notificationToggle = MyApp.notification;

  updateVibration() {
    if (vibrationToggle) {
      setState(() {
        vibrationToggle = false;
        MyApp.vibration = false;
      });
    } else {
      setState(() {
        vibrationToggle = true;
        MyApp.vibration = true;
      });
    }
    //update flag in firebase
    if (MyApp.user != null) {
      if (MyApp.pin != null) {
        var db = MyApp.database.reference().child('settings')
            .child(MyApp.pin)
            .update(
            <String, String>{
              "vibration": "" + (vibrationToggle ? '1' : '0'),
            })
            .then((result) {
          print("INFO: Database Write Completed");
        });
      }
    }
  }

  updateNotifications() {
    if (notificationToggle) {
      setState(() {
        notificationToggle = false;
        MyApp.notification = false;
      });
    } else {
      setState(() {
        notificationToggle = true;
        MyApp.notification = true;
      });
    }
    if (MyApp.user != null) {
      if (MyApp.pin != null) {
        var db = MyApp.database.reference().child('settings')
            .child(MyApp.pin)
            .update(
            <String, String>{
              "notification": "" + (notificationToggle ? '1' : '0'),
            })
            .then((result) {
          print("INFO: Database Write Completed");
        });
      }
    }
  }

  Future sendNotification() async {
    if (MyApp.notification && notificationToggle) {
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
          0, 'New Post', 'How to Show Notification in flutter',
          platformChannelSpecifics, payload: 'Default_Sound');
    } else {
      print('Notifications turned off.');
    }
  }


  Widget listViewConstructor(BuildContext context) {
    return ListView(
      children: <Widget>[
        ListTile(
            title: Text('Toggle Chair Vibration'),
            trailing: Switch(
                value: vibrationToggle, onChanged: (value) => updateVibration())
        ),
        ListTile(
          title: Text('Toggle Phone Notifications'),
          trailing: Switch(value: notificationToggle,
              onChanged: (value) => updateNotifications()),
        ),
        ListTile(
          title: Text('Test Notification!'),
          onTap: () => sendNotification(),
        )
/*        ListTile(
          title: Text('Sign Out'),
         onTap: () => signOut(context),
        )*/
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    return new Scaffold(
        appBar: AppBar(
          title: const Text('Settings'),
        ),
        body: SwipeDetector(
          child: listViewConstructor(context),
          onSwipeRight: () =>
              Navigator.pushReplacementNamed(context, '/DashboardScreen'),
          swipeConfiguration: SwipeConfiguration(
              horizontalSwipeMaxHeightThreshold: 50.0,
              horizontalSwipeMinDisplacement: 10.0,
              horizontalSwipeMinVelocity: 1.0,
              verticalSwipeMaxWidthThreshold: 100.0,
              verticalSwipeMinDisplacement: 50.0,
              verticalSwipeMinVelocity: 100.0
          ),
        )
    );
  }
}