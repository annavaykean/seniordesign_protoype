import 'package:flutter/material.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:sd_proto/main.dart';
import 'package:intl/intl.dart';
import 'package:charts_flutter/flutter.dart';

//send and receive numbers (for testing purposes only)
class DatabaseTestPage extends StatefulWidget {
  @override
  DatabaseTestPageState createState() => DatabaseTestPageState();
}

class DatabaseTestPageState extends State<DatabaseTestPage> {
  TextEditingController cogXctrl = new TextEditingController();
  TextEditingController cogYctrl = new TextEditingController();
  List list = [];

  signOut(BuildContext context) {
    MyApp.firebaseAuth.signOut();
    MyApp.user = null;
    Navigator.popUntil(context, ModalRoute.withName('/'));
  }

  writeToDatabase(String cogX, String cogY){
    //prep data
    DateTime now = DateTime.now();
    String formattedDate = DateFormat('MMddyyyy_kkmmss').format(now);
    //send data
    print("INFO: Preparing to write to database...");
    if(MyApp.user != null){
      MyApp.userReference.child(MyApp.user.uid).child(formattedDate).set(<String, String>{
        "cogX": "" + cogX,
        "cogY": "" + cogY,
        "created_at": "" + formattedDate
      }).then((result){
        print("INFO: Database Write Completed");
      });
    }
  }

  readFromDatabase(){
    list = [];
    var userID = MyApp.user.uid;
    MyApp.userReference.child(userID).once().then((DataSnapshot snapshot) {
      print('DATA: ${snapshot.value}');
      for(var value in snapshot.value.values) {
        list.add(new Posture.fromJson(value));
      }
      print('length: ' + list.length.toString());
   //   return list;
      print('hit');
      showData();
/*      return ListView.builder(
        padding: const EdgeInsets.all(16.0),
        itemBuilder: (context, i) {
          print('INFO: ' + list.length.toString());
          if (list.length > i) {
            print('hit2');
            return buildRow(list[i]);
          }
        },
        shrinkWrap: true,
      );*/
    });

  }

  buildRow(var data) {
    String summary = 'CogX: ' + data.cogX + ', CogY: ' + data.cogY + ', date: ' + data.created_at;
    print('SUMMARY: ' + summary);
    return ListTile(
        title: Text(summary),
        onTap: () {
          print('tap');
        }
    );
  }
  showData() {
 //   readFromDatabase().then((list) {
    if(list != null && list.length > 0) {
      print(list.length);
      return ListView.builder(
        padding: const EdgeInsets.all(16.0),
        itemBuilder: (context, i) {
          print('INFO: ' + list.length.toString());
          if (list.length > i) {
            print('hit2');
            return buildRow(list[i]);
          }
        },
        shrinkWrap: true,
      );
//    });
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
              ),
              RaisedButton(
                child: const Text('Fetch Data from Database'),
                onPressed: () => readFromDatabase(), //showData(),
              ),
              Expanded(
                child: Container(
                  child: showData(),
                )
              )
            ]
        )
    );
  }

}

class Posture {
  String cogX;
  String cogY;
  String created_at;

  Posture.fromJson(var data){
    this.cogX = data['cogX'];
    this.cogY = data['cogY'];
    this.created_at = data['created_at'];
  }
}



