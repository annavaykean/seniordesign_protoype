import 'package:flutter/material.dart';
import 'package:sd_proto/main.dart';

class Stretches extends StatelessWidget {

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
              child: Image.asset(MyApp.pose),
            ),
            Text('Pose above'),
          ],
        )
    );
  }
}