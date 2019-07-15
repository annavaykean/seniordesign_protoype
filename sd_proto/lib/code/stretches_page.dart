import 'package:flutter/material.dart';
import 'package:sd_proto/main.dart';

class Stretches extends StatelessWidget {

  @override
  Widget build(BuildContext context) {
    return new Scaffold(
        appBar: AppBar(
          title: Text(MyApp.poseLabel),
        ),
        body: Center(
              child: Container(
                child: Image.asset(MyApp.pose),
              ),
        )
    );
  }
}