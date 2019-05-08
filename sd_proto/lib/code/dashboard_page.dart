import 'package:flutter/material.dart';

class DashboardScreen extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return new Scaffold(
        appBar: AppBar(
          title: const Text('This is a new page!'),
        ),
        body: Center(
            child: RaisedButton(
              child: const Text('Show me the settings!'),
              onPressed: () => Navigator.of(context).pushNamed('/SettingsScreen'),
            )
        )
    );
  }
}