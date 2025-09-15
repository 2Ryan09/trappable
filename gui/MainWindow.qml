import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 400
    height: 300
    title: "Trappable"

    Column {
        anchors.centerIn: parent
        spacing: 16

        TextField {
            id: firstNumber
            placeholderText: "Enter first number"
            width: 200
            inputMethodHints: Qt.ImhFormattedNumbersOnly
        }
        TextField {
            id: secondNumber
            placeholderText: "Enter second number"
            width: 200
            inputMethodHints: Qt.ImhFormattedNumbersOnly
        }
        Button {
            text: "Add"
            onClicked: {
                var a = parseFloat(firstNumber.text);
                var b = parseFloat(secondNumber.text);
                if (!isNaN(a) && !isNaN(b)) {
                    resultLabel.text = "Result: " + (a + b);
                } else {
                    resultLabel.text = "Please enter valid numbers.";
                }
            }
        }
        Label {
            id: resultLabel
            text: "Result: "
        }
    }
}
