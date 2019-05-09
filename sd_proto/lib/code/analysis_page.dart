import 'package:flutter/material.dart';

class AnalysisScreen extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return new Scaffold(
        appBar: AppBar(
          title: const Text('This will be the analysis page!'),
        ),
        body: Column(
            children: <Widget> [
            RaisedButton(
              child: const Text('Sign Out'),
              onPressed: () => Navigator.popUntil(context, ModalRoute.withName('/')),
            ),
              RaisedButton(
                child: const Text('Dashboard'),
                onPressed: () => Navigator.of(context).pushNamed('/DashboardScreen'),
              )
          ]
        )
    );
  }
}