import 'package:flutter/material.dart';

class DashboardScreen extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return new Scaffold(
        appBar: AppBar(
          title: const Text('This is a home page!'),
        ),
        body: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            crossAxisAlignment: CrossAxisAlignment.stretch,
            children: <Widget>[
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
              )
            ]
        )
    );
  }
}