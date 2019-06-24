import 'package:flutter/material.dart';
import 'package:sd_proto/main.dart';
import 'package:charts_flutter/flutter.dart' as charts;
import 'package:firebase_database/firebase_database.dart';
import 'package:intl/intl.dart';
import 'dart:math';
import 'package:swipedetector/swipedetector.dart';
class DashboardScreen extends StatelessWidget {
  //List<GraphData> data = MyApp.postureDataList;

  List<GraphData> data = null;
  List<charts.Series<GraphData, int>> series;

  signOut(BuildContext context) {
    if(MyApp.firebaseAuth != null) {
      MyApp.firebaseAuth.signOut();
    }
    if(MyApp.user != null) {
      MyApp.user = null;
    }
    Navigator.of(context).pushReplacementNamed('/WelcomeScreen');
    if(MyApp.deviceConnection != null) {
      MyApp.deviceConnection.cancel();
    }

  }


  Widget buildChart(){
    data = [];
    print('in build chart');
    if(MyApp.postureDataList == null) {
      print('pdl is null in build chart');
      return Text('No Data Available');
    }
    else {
      print('in build chart: length ' + MyApp.postureDataList.length.toString());
      for(int i=0;i<MyApp.postureDataList.length;i++) {
        //convert posture data object to graph data object
        var freshData = new GraphData(MyApp.postureDataList[i].cogX, MyApp.postureDataList[i].cogY, MyApp.postureDataList[i].created_at);
        if(freshData != null && data != null) {
          data.add(freshData);
          print('added freshData to data list');
        }
        else {
          return Text('Invalid Data. Cannot display.');
        }
      }
      series = fetchGraphData();
      return charts.ScatterPlotChart(
          series,
          animate: true,
      );
    }
  }
  readFromDatabase(){
    print('hit');
    List list = [];
    var userReference = MyApp.database.reference().child('0000').child('data');
    userReference.once().then((DataSnapshot snapshot) {
      print('DATA: ${snapshot.value}');
      for(var value in snapshot.value.values) {
        //was previously experiencing errors on parsing from json. Fixed by converting data to string then to int.
        var cogX = value['cogX'].toString();
        var xInt = int.parse(cogX);
        var cogY = value['cogY'].toString();
        var yInt = int.parse(cogY);
        var created_at = value['created_at'].toString();

        //add parsed data to list as a Posture object
        list.add(new GraphData(xInt, yInt, created_at));
      }
      print('exited for loop');
      //update global posture data list with fresh data.
      MyApp.postureDataList = list;
      //display list on db testing page for testing purposes...disable later
      //   showData();
      print('end2');
      series = fetchGraphData();
      buildChart();
    });

  }

  void reload(BuildContext context) {
    //reload chart with fresh data
    if(MyApp.postureDataList == null) {
      print('pdL is null');
      //pull fresh data from database
       MyApp.databaseData.readFromDatabase();
        print('hit2');
        print(MyApp.postureDataList);
        series = fetchGraphData();
        buildChart();
        print(data);
    } else {
      print('pdl is not null');
      buildChart();
    }
    Navigator.of(context).pushReplacementNamed('/DashboardScreen');
  }

  goToSettings (BuildContext context) {
    print('hit');
    var userReference = MyApp.database.reference().child('settings').child(MyApp.pin);
    //pull posture data
    userReference.once().then((DataSnapshot snapshot) {
      if(snapshot.value['vibration'] == '1'){
        print('vibration feature turned on');
        MyApp.vibration = true;

      } else {
        print('vibration feature turned off');
        MyApp.vibration = false;
      }
      if(snapshot.value['notification'] == '1') {
        print('notification feature turned on');
        MyApp.notification = true;
      } else {
        print('notification feature turned off');
        MyApp.notification = false;
      }
      Navigator.of(context).pushNamed('/SettingsScreen');
      });
  }

  displayStretchPage(BuildContext context) {
    List imageURLs = ['cat-cow.jpg', 'thread-the-needle.jpg', 'supine-twist.jpg', 'arm-across-chest.jpg', 'childs-pose.jpg', 'eagle-arms.jpg', 'sphinx-pose.jpg'];

    //generate random number to select index of image
    var random = new Random();
    int scaledRandom = random.nextInt((imageURLs.length) - 0);

    //return image to display
    if (scaledRandom >= 0 && scaledRandom < imageURLs.length) {
      MyApp.pose = 'assets/' + imageURLs[scaledRandom];
      Navigator.pushNamed(context, '/Stretches');
    }
  }
  @override
  Widget build(BuildContext context) {
     series = fetchGraphData();
    return new Scaffold(
        appBar: AppBar(
          title: Text('Welcome ${MyApp.user.email}'),
            actions: <Widget>[
              new IconButton(
                icon: new Icon(Icons.threesixty),
                onPressed: () => reload(context),
              ),
            new IconButton(
              icon: new Icon(Icons.close),
              onPressed: () => signOut(context),
            )
  ],
        ),
        body: SwipeDetector(
              child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            crossAxisAlignment: CrossAxisAlignment.stretch,
            children: <Widget>[
              Expanded(
                child: buildChart(),
              ),
              RaisedButton(
                child: Text('I am experiencing back pain!'),
                onPressed: () => displayStretchPage(context),
              ),
              RaisedButton(
                child: const Text('Show me the settings page!'),
                onPressed: () => goToSettings(context),
              ),
              RaisedButton(
                child: const Text('Database Testing Page'),
                onPressed: () => Navigator.of(context).pushNamed('/DatabaseTestPage'),
              ),
            ]
        ),
        onSwipeLeft: () => goToSettings(context),
        // onSwipeRight: () => goToDataPage(context),
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

   List<charts.Series<GraphData, int>> fetchGraphData() {
    print('fetching graph data');
    return [
      new charts.Series<GraphData, int> (
        id: 'Posture',
        colorFn:(_, __) => charts.MaterialPalette.blue.shadeDefault,
        domainFn: (GraphData dataPoint, _) => dataPoint.cogX,
        measureFn: (GraphData dataPoint, _) => dataPoint.cogY,
        data: data,
      )
    ];
  }
}

class GraphData {
   final int cogX;
   final int cogY;
   final String created_at;

   GraphData(this.cogX, this.cogY, this.created_at);

}