import 'package:flutter/material.dart';
import 'package:sd_proto/main.dart';
class DashboardScreen extends StatelessWidget {

  signOut(BuildContext context) {
    MyApp.firebaseAuth.signOut();
    MyApp.user = null;
    Navigator.popUntil(context, ModalRoute.withName('/'));
  }

  @override
  Widget build(BuildContext context) {
    return new Scaffold(
        appBar: AppBar(
          title: Text('Welcome ${MyApp.user.email}'),
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
}