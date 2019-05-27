import 'package:flutter/material.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:sd_proto/main.dart';
import 'package:intl/intl.dart';
//send and receive numbers (for testing purposes only)
class DatabaseTestPage extends StatefulWidget {
  @override
  DatabaseTestPageState createState() => DatabaseTestPageState();
}

class DatabaseTestPageState extends State<DatabaseTestPage> {
  TextEditingController cogXctrl = new TextEditingController();
  TextEditingController cogYctrl = new TextEditingController();


  signOut(BuildContext context) {
    MyApp.firebaseAuth.signOut();
    MyApp.user = null;
    Navigator.popUntil(context, ModalRoute.withName('/'));
  }

  writeToDatabase(String cogX, String cogY){

    //prep data
    DateTime now = DateTime.now();
    String formattedDate = DateFormat('MMddyyyy_kkmm').format(now);
    int cogX_num = int.parse(cogX);
    int cogY_num = int.parse(cogY);

    //send data
    print("INFO: Preparing to write to database...");
    if(MyApp.user != null){
    MyApp.userReference.push().set(<String, String>{
      "cogX": "" + cogX,
      "cogY": "" + cogY,
      "created_at": "" + formattedDate
    }).then((result){
      print("INFO: Database Write Completed");
    });
    }
  }

  @override
  Widget build(BuildContext context) {
    return new Scaffold(
        appBar: AppBar(
          title: Text('Database Test Page'),
        ),
        body: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            crossAxisAlignment: CrossAxisAlignment.stretch,
            children: <Widget>[
              RaisedButton(
                child: const Text('Sign Out'),
                onPressed: () => signOut(context),
              ),
              Container(
                child: new TextField(
                  controller: cogXctrl,
                  autofocus: true,
                  decoration: new InputDecoration(
                    labelText: 'CogX',
                  ),
                )
              ),
              Container(
                child: new TextField(
                  controller: cogYctrl,
                  autofocus: false,
                  decoration: new InputDecoration(
                    labelText: 'CogY'
                  )
                )
              ),
              RaisedButton(
                child: const Text('Submit to Database'),
                onPressed: () => writeToDatabase(cogXctrl.text, cogYctrl.text)
              )
            ]
        )
    );
  }
}

