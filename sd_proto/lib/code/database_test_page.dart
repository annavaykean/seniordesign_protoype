import 'package:flutter/material.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:sd_proto/main.dart';
import 'package:intl/intl.dart';
import 'package:charts_flutter/flutter.dart' as charts;
import 'dart:convert';
//send and receive numbers (for testing purposes only)
class DatabaseTestPage extends StatefulWidget {
  @override
  DatabaseTestPageState createState() => DatabaseTestPageState();
}

class DatabaseTestPageState extends State<DatabaseTestPage> {
  TextEditingController cogXctrl = new TextEditingController();
  TextEditingController cogYctrl = new TextEditingController();
  //List list = [];
  List list = MyApp.postureDataList;


  signOut(BuildContext context) {
    if(MyApp.firebaseAuth != null) {
      MyApp.firebaseAuth.signOut();
    }
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

  //    MyApp.userReference.child(MyApp.user.uid).child(formattedDate).set(<String, String>{
      MyApp.userReference.child(formattedDate).set(<String, String>{
        "cogX": "" + cogX,
        "cogY": "" + cogY,
        "created_at": "" + now.toString(),
      }).then((result){
        print("INFO: Database Write Completed");
      });
    }
  }

  readFromDatabase(){
    print('reading from firebase...');
    list = [];
    //var userID = MyApp.user.uid;
    MyApp.userReference.once().then((DataSnapshot snapshot) {
      print('DATA: ${snapshot.value}');
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
      //display list on db testing page for testing purposes...disable later
      showData();
    });

  }

  buildRow(var data) {
    String summary = 'CogX: ' + data.cogX.toString() + ', CogY: ' + data.cogY.toString(); //+ ', date: ' + data.created_at;
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
                  autofocus: false,
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
              Container(
                  child: null,
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
  int cogX;
  int cogY;
  String created_at;
Posture(this.cogX, this.cogY, this.created_at);

}




