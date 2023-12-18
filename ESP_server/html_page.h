const char webpageCode[] PROGMEM =
  R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>WebSocket LED Control</title>
    <link href="/path/to/bootstrap.min.css" rel="stylesheet">
    <link href="dist/css/component-custom-switch.css" rel="stylesheet">
    <style>
                .toggle {

            --width: 80px;
            --height: calc(var(--width) / 3);

            position: relative;
            display: inline-block;
            width: var(--width);
            height: var(--height);
            box-shadow: 0px 1px 3px rgba(0, 0, 0, 0.3);
            border-radius: var(--height);
            cursor: pointer;
        }

        .toggle input {
            display: none;
        }

        .toggle .slider {
            position: absolute;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            border-radius: var(--height);
            background-color: #ccc;
            transition: all 0.4s ease-in-out;
        }

        .toggle .slider::before {
            content: '';
            position: absolute;
            top: 0;
            left: 0;
            width: calc(var(--height));
            height: calc(var(--height));
            border-radius: calc(var(--height) / 2);
            background-color: #fff;
            box-shadow: 0px 1px 3px rgba(0, 0, 0, 0.3);
            transition: all 0.4s ease-in-out;
        }

        .toggle.on input:checked+.slider {
            background-color: #2196F3;
        }

        .toggle.on input:checked+.slider::before {
            transform: translateX(calc(var(--width) - var(--height)));
        }

        .toggle .labels {
            position: absolute;
            top: 8px;
            left: 0;
            width: 100%;
            height: 100%;
            font-size: 12px;
            font-family: sans-serif;
            transition: all 0.4s ease-in-out;
        }

        .toggle.off .labels::after {
            content: attr(data-off);
            position: absolute;
            right: 5px;
            color: #4d4d4d;
            opacity: 1;
            text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.4);
            transition: all 0.4s ease-in-out;
        }

        .toggle.on .labels::before {
            content: attr(data-on);
            position: absolute;
            left: 5px;
            color: #ffffff;
            opacity: 0;
            text-shadow: 1px 1px 2px rgba(255, 255, 255, 0.4);
            transition: all 0.4s ease-in-out;
        }

        .toggle input:checked~.labels::after {
            opacity: 0;
        }

        .toggle input:checked~.labels::before {
            opacity: 1;
        }
    </style>
</head>

<body>
    <h1>led1</h1>
    <label class="toggle on off" id="led1">
        <input type="checkbox" onclick="toggleLed('led1')()">
        <span class="slider"></span>
        <span class="labels" data-on="ON" data-off="OFF"></span>
    </label>
    <h1>led2</h1>
    <label class="toggle on off" id="led2">
        <input type="checkbox" onclick="toggleLed('led2')">
        <span class="slider"></span>
        <span class="labels" data-on="ON" data-off="OFF"></span>
    </label>


    <script>
        const socket = new WebSocket('ws://' + window.location.hostname + ':81/');

        // Sự kiện khi kết nối được mở
        socket.addEventListener('open', (event) => {
            console.log('WebSocket connection opened.');
        });

        // Sự kiện khi nhận được dữ liệu từ server
        socket.addEventListener('message', (event) => {
            console.log('Received message from server:', event.data);

            // Xử lý trạng thái của đèn từ dữ liệu nhận được
            const ledStatus = JSON.parse(event.data);
            var led1String = document.getElementById("led1");
            var led2String = document.getElementById("led2");
            if (ledStatus.led1 === "ON")  {
                led1String.classList.remove("off");
                led1String.classList.add("on");
                ledState = true;
            } else {
                led1String.classList.remove("on");
                led1String.classList.add("off");
                 ledState = false;
            }
            if (ledStatus.led2 === "ON") {
                led2String.classList.remove("off");
                led2String.classList.add("on");
                 ledState = true;
            } else {
                led2String.classList.remove("on");
                led2String.classList.add("off");
                 ledState = false;
            }
        });

        // Hàm gửi yêu cầu điều khiển đèn
        function toggleLed(led) {
            const action = `toggle${led.charAt(0).toUpperCase()}${led.slice(1)}`;
            const request = { action: action };

            // Chuyển đối tượng thành chuỗi JSON
            const jsonString = JSON.stringify(request);

            // Gửi chuỗi JSON tới server qua WebSocket
            socket.send(jsonString);
        }

    </script>
</body>

</html>
)=====";
