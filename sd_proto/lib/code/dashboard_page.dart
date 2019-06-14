import 'package:flutter/material.dart';
import 'package:sd_proto/main.dart';
import 'package:charts_flutter/flutter.dart' as charts;
import 'package:firebase_database/firebase_database.dart';
import 'package:intl/intl.dart';



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
    Navigator.of(context).pushNamed('/WelcomeScreen');
    if(MyApp.deviceConnection != null) {
      MyApp.deviceConnection.cancel();
    }

  }


  Widget buildChart(){
    data = [];
    series = fetchGraphData();
    if(MyApp.postureDataList == null) {
      return Text('No Data Available');
    }
    else {
      print('length test: ' + MyApp.postureDataList.length.toString());
      for(int i=0;i<MyApp.postureDataList.length;i++) {
        var freshData = new GraphData(MyApp.postureDataList[i].cogX, MyApp.postureDataList[i].cogY, MyApp.postureDataList[i].created_at);
        if(freshData != null && data != null) {
          print('adding data...');
          data.add(freshData);
        }
        else {
          return Text('Invalid Data. Cannot display.');
        }
      }
      //sort by date

      print('building chart now...');
      return charts.ScatterPlotChart(
          series,
          animate: true,
 /*         behaviors: [
            new charts.InitialSelection(selectedDataConfig: [
              new charts.SeriesDatumConfig<String>('CogX', 'testing')
            ])
          ],*/
      );
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
              icon: new Icon(Icons.close),
              onPressed: () => signOut(context),
            )
  ],
        ),
        body: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            crossAxisAlignment: CrossAxisAlignment.stretch,
            children: <Widget>[
              Expanded(
                child: buildChart(),
              ),
              RaisedButton(
                child: const Text('Show me the settings page!'),
                onPressed: () => Navigator.of(context).pushNamed('/SettingsScreen'),
              ),
              RaisedButton(
                child: const Text('Show me the anaylsis page!'),
                onPressed: () => Navigator.of(context).pushNamed('/AnalysisScreen'),
              ),
              RaisedButton(
                child: const Text('Bluetooth Testing Page'),
                onPressed: () => Navigator.of(context).pushNamed('/BluetoothTestScreen'),
              ),
              RaisedButton(
                child: const Text('Sign Out'),
                onPressed: () => signOut(context),
              ),
              RaisedButton(
                child: const Text('Database Testing Page'),
                onPressed: () => Navigator.of(context).pushNamed('/DatabaseTestPage'),
              )
            ]
        )
    );
  }

   List<charts.Series<GraphData, int>> fetchGraphData() {
    print('fetching graph data...');
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