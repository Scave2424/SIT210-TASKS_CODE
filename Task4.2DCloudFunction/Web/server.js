// Import Express so this file can create the backend web server.
const express = require("express");

const app = express();
const PORT = 3000;

// Store the IP address currently assigned to the Arduino.
const ARDUINO_IP = "192.168.29.244";

// Serve index.html and other files from the public folder.
app.use(express.static("public"));

// Receive a room request from index.html and forward it to the Arduino.
app.get("/toggle", async (req, res) => {

    const room = req.query.room;
    const allowedRooms = ["livingroom", "bathroom", "closet"];

    // Reject any room name that is not used in this project.
    if (!allowedRooms.includes(room)) {
        return res.status(400).send("Invalid room");
    }

    try {

        // Send the selected room command to the Arduino over the local network.
        const response = await fetch(
            `http://${ARDUINO_IP}/toggle?room=${room}`
        );

        const message = await response.text();

        // Return the Arduino response back to the webpage.
        res.send(message);

    } catch (error) {

        // Report a connection failure if the backend cannot reach the Arduino.
        console.error("Arduino connection error:", error);
        res.status(500).send("Arduino connection failed");
    }
});

// Start the backend server on port 3000.
app.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}`);
});