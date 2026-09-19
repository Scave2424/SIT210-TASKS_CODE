import tkinter as tkin
import RPi.GPIO as GPIO

# Set the GPIO numbering mode to use the Raspberry Pi BCM GPIO numbers.
GPIO.setmode(GPIO.BCM)

# Assign one GPIO pin to each room light.
LIVING_ROOM = 17
BATHROOM = 22
CLOSET = 27

# Configure the three GPIO pins as outputs for controlling the LEDs.
GPIO.setup(LIVING_ROOM, GPIO.OUT)
GPIO.setup(BATHROOM, GPIO.OUT)
GPIO.setup(CLOSET, GPIO.OUT)

# Keep all three LEDs switched off when the program starts.
GPIO.output(LIVING_ROOM, GPIO.LOW)
GPIO.output(BATHROOM, GPIO.LOW)
GPIO.output(CLOSET, GPIO.LOW)


# Control the LEDs according to the room selected in the GUI.
def change_room():
    # Switch off all LEDs before activating the selected room.
    GPIO.output(LIVING_ROOM, GPIO.LOW)
    GPIO.output(BATHROOM, GPIO.LOW)
    GPIO.output(CLOSET, GPIO.LOW)

    # Get the room currently selected by the user.
    selected_room = room.get()

    # Turn on the Living Room LED when the Living Room is selected.
    if selected_room == "Living Room":
        GPIO.output(LIVING_ROOM, GPIO.HIGH)

    # Turn on the Bathroom LED when the Bathroom is selected.
    elif selected_room == "Bathroom":
        GPIO.output(BATHROOM, GPIO.HIGH)

    # Turn on the Closet LED when the Closet is selected.
    elif selected_room == "Closet":
        GPIO.output(CLOSET, GPIO.HIGH)


# Switch off all LEDs and close the GUI when Exit is selected.
def exit_gui():
    # Switch off every LED before closing the application.
    GPIO.output(LIVING_ROOM, GPIO.LOW)
    GPIO.output(BATHROOM, GPIO.LOW)
    GPIO.output(CLOSET, GPIO.LOW)

    # Close the GUI window.
    window.destroy()


# Create the main GUI window.
window = tkin.Tk()
window.title("Smart Home Lighting")
window.geometry("400x350")


# Create the heading displayed at the top of the GUI.
title = tkin.Label(
    window,
    text="Smart Home Lighting",
    font=("Arial", 20, "bold")
)

# Place the heading inside the GUI with vertical spacing.
title.pack(pady=20)


# Store the room selected through the radio buttons.
room = tkin.StringVar()
room.set("None")


# Create the radio button for the Living Room.
living_button = tkin.Radiobutton(
    window,
    text="Living Room",
    variable=room,
    value="Living Room",
    command=change_room,
    font=("Arial", 14)
)

# Place the Living Room option in the GUI.
living_button.pack(pady=5)


# Create the radio button for the Bathroom.
bathroom_button = tkin.Radiobutton(
    window,
    text="Bathroom",
    variable=room,
    value="Bathroom",
    command=change_room,
    font=("Arial", 14)
)

# Place the Bathroom option in the GUI.
bathroom_button.pack(pady=5)


# Create the radio button for the Closet.
closet_button = tkin.Radiobutton(
    window,
    text="Closet",
    variable=room,
    value="Closet",
    command=change_room,
    font=("Arial", 14)
)

# Place the Closet option in the GUI.
closet_button.pack(pady=5)


# Create the button used to close the GUI.
exit_button = tkin.Button(
    window,
    text="Exit",
    command=exit_gui,
    font=("Arial", 14),
    width=12
)

# Place the Exit button below the room options.
exit_button.pack(pady=25)


# Start the GUI and keep it running until the user exits.
try:
    window.mainloop()

# Reset the GPIO pins after the GUI is closed.
finally:
    GPIO.cleanup()