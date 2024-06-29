# PostureGuard

PostureGuard is an IoT-based posture monitoring and correction system designed for home workouts. This project is part of our first-year university course IN 1901 - Microcontroller Based Application Development at the University of Moratuwa.

## Features
- **Vest System:** Monitors heart rate and flex sensor data to track upper body posture.
- **Mats System:** Measures weight distribution across load cells to determine the user's center of gravity.
- **Real-time Data Visualization:** Utilizes Socket.io for live updates, allowing users to see instantaneous feedback on their posture.
- **User Configurability:** Includes adjustable settings for flex sensor ranges and weight difference thresholds, enabling personalized monitoring.
- **Intuitive Web Interface:** Designed with a dark theme and clear data presentation to enhance user experience.

## How It Works
PostureGuard uses ESP32 microcontrollers to gather data from various sensors and sends this data to a web server. The server processes the data and updates the web interface in real-time, providing immediate feedback to the user.

## Getting Started
To get started with PostureGuard, follow these steps:
1. Clone the repository: `git clone https://github.com/your-username/PostureGuard.git`
2. Install the necessary dependencies: `npm install`
3. Start the server: `node server.js`
4. Open your web browser and navigate to `http://localhost:3000`

## Contributing
Contributions are welcome! Please open an issue or submit a pull request.

## License
This project is licensed under the MIT License. See the `LICENSE` file for more details.

## Contact
For more information, visit our [LinkedIn](https://www.linkedin.com/in/h-k-sahan-tharaka-73344728a/).


 
