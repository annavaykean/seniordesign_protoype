import 'package:flutter/material.dart';
import 'package:sd_proto/main.dart';
import 'package:charts_flutter/flutter.dart' as charts;
import 'package:firebase_database/firebase_database.dart';
import 'package:intl/intl.dart';
import 'dart:math';
class Stretches extends StatelessWidget {

  recommendPose () {
    //selection of pose is being performed on button click in the dashboard_page.dart file
    //was previously doing it here, but took too long and didn't load image fast enough
    //for now, let's randomly pick a pose to display
    //in the future, use posture data to tailor pose selection
      return Image.asset(MyApp.pose);
  }


  @override
  Widget build(BuildContext context) {
    return new Scaffold(
        appBar: AppBar(
          title: const Text('Try this pose!'),
        ),
        body: Column(
          children: <Widget>[
            //Image.asset('assets/cat-cow.jpg'),
            Container(
              child: recommendPose(),
            ),
            Text('Pose above'),
          ],
        )
    );
  }
}