import 'package:flutter/material.dart';

class BluetoothTestScreen extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return new Scaffold(
        appBar: AppBar(
          title: const Text('This is for testing Bluetooth!'),
        ),
        body: Column(
            children: <Widget> [
              RaisedButton(
                child: const Text('Search for devices'),
                onPressed: null,
              )
            ]
        )
    );
  }
}