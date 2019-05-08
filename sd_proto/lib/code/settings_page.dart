import 'package:flutter/material.dart';

class SettingsScreen extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return new Scaffold(
        appBar: AppBar(
          title: const Text('This will be the settings page!'),
        ),
        body: Center(
            child: RaisedButton(
              child: const Text('Go Home!'),
              onPressed: () => Navigator.of(context).pushNamed('/WelcomeScreen'),
            )
        )
    );
  }
}