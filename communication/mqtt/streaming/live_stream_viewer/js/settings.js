MQTT_BROKER = "broker.mqttdashboard.com";  
MQTT_PORT = 8000;
USERNAME = null;
PASSWORD = null;
MQTT_PATH = "/mqtt";
MQTT_TOPIC = "camera/elhamshaMQTTStream";  
RECONNECT_TIMEOUT = 2000;
USE_TLS = false;
CLEAN_SESSION = true;

// MQTT_BROKER = "broker.hivemq.com";  
// MQTT_PORT = 8083;

var mqttConnVar;
function mqttConnect() {
	if (typeof MQTT_PATH == "undefined") {
		MQTT_PATH = "/mqtt";
	}
	mqttConnVar = new Paho.MQTT.Client(MQTT_BROKER, MQTT_PORT, MQTT_PATH
		, "web_" + parseInt(Math.random() * 100, 10));
	var options = {
		timeout: 3,
		useSSL: USE_TLS,
		cleanSession: CLEAN_SESSION,
		onSuccess: onConnect,
		onFailure: function (message) {
			$("#status").val(
				"Connection failed: " + message.errorMessage + " retrying"
			);
			setTimeout(mqttConnect, RECONNECT_TIMEOUT);
		},
	};

	mqttConnVar.onConnectionLost = onConnectionLost;
	mqttConnVar.onMessageArrived = onMessageArrived;

	if (USERNAME != null) {
		options.userName = USERNAME;
		options.password = PASSWORD;
	}
	console.log(
		"mqtt_broker = " + MQTT_BROKER +
		", mqtt_port = " + MQTT_PORT +
		", mqtt_path = " + MQTT_PATH +
		", TLS = " + USE_TLS +
		", username = " + USERNAME +
		", password = " + PASSWORD
	);
	mqttConnVar.connect(options);
}

function onConnect() {
	$("#status").val("Connected to " + MQTT_BROKER + ":" + MQTT_PORT + MQTT_PATH);
	mqttConnVar.subscribe(MQTT_TOPIC, { qos: 0 });
	$("#mqtt_topic").val(MQTT_TOPIC);
}

function onConnectionLost(response) {
	setTimeout(mqttConnect, RECONNECT_TIMEOUT);
	$("#status").val(
		"--> Connection lost: " + responseObject.errorMessage + ". reconnecting "
	);
}

function onMessageArrived(message) {
	var topic = message.destinationName;
	var payload = message.payloadString;

	if (payload != undefined && payload.length > 0) {
		$("#webcam").attr("src", payload);
		console.log(typeof(payload));
	}
}

$(document).ready(function () {
	mqttConnect();
});

