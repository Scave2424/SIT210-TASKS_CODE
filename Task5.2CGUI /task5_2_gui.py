import tkinter as tkin
import RPi.GPIO as GPIO

# Use BCM numbers for the GPIO pins.
GPIO.setmode(GPIO.BCM)

# GPIO pins used for the three lights.
LIVING_ROOM = 12
BATHROOM = 13
CLOSET = 18

# Set the LED pins as outputs.
GPIO.setup(LIVING_ROOM, GPIO.OUT)
GPIO.setup(BATHROOM, GPIO.OUT)
GPIO.setup(CLOSET, GPIO.OUT)

# Set up PWM for each LED.
living_pwm = GPIO.PWM(LIVING_ROOM, 1000)
bathroom_pwm = GPIO.PWM(BATHROOM, 1000)
closet_pwm = GPIO.PWM(CLOSET, 1000)

# Start the LEDs at 0% brightness.
living_pwm.start(0)
bathroom_pwm.start(0)
closet_pwm.start(0)


# Change the brightness of the selected light.
def change_intensity(value):
    selected_room = room.get()

    if selected_room == "Living Room":
        living_pwm.ChangeDutyCycle(float(value))
    elif selected_room == "Bathroom":
        bathroom_pwm.ChangeDutyCycle(float(value))
    elif selected_room == "Closet":
        closet_pwm.ChangeDutyCycle(float(value))


# Change the light when a different room is selected.
def change_room():
    # Turn the other lights off.
    living_pwm.ChangeDutyCycle(0)
    bathroom_pwm.ChangeDutyCycle(0)
    closet_pwm.ChangeDutyCycle(0)

    selected_room = room.get()

    if selected_room == "Living Room":
        living_pwm.ChangeDutyCycle(float(living_intensity.get()))
    elif selected_room == "Bathroom":
        bathroom_pwm.ChangeDutyCycle(float(bathroom_intensity.get()))
    elif selected_room == "Closet":
        closet_pwm.ChangeDutyCycle(float(closet_intensity.get()))


# Turn everything off before closing.
def exit_gui():
    living_pwm.ChangeDutyCycle(0)
    bathroom_pwm.ChangeDutyCycle(0)
    closet_pwm.ChangeDutyCycle(0)

    living_pwm.stop()
    bathroom_pwm.stop()
    closet_pwm.stop()

    window.destroy()


# Create the main window.
window = tkin.Tk()
window.title("Smart Home Lighting")
window.geometry("450x600")

# Add the title at the top.
title = tkin.Label(
    window,
    text="Smart Home Lighting",
    font=("Arial", 20, "bold")
)
title.pack(pady=20)

# Keep track of the selected room.
room = tkin.StringVar()
room.set("None")


# Living Room option.
living_button = tkin.Radiobutton(
    window,
    text="Living Room",
    variable=room,
    value="Living Room",
    command=change_room,
    font=("Arial", 14)
)
living_button.pack(pady=5)

# Living Room brightness control.
living_intensity = tkin.Scale(
    window,
    from_=0,
    to=100,
    orient=tkin.HORIZONTAL,
    label="Living Room Intensity (%)",
    command=change_intensity,
    length=300
)
living_intensity.set(50)
living_intensity.pack(pady=10)


# Bathroom option.
bathroom_button = tkin.Radiobutton(
    window,
    text="Bathroom",
    variable=room,
    value="Bathroom",
    command=change_room,
    font=("Arial", 14)
)
bathroom_button.pack(pady=5)

# Bathroom brightness control.
bathroom_intensity = tkin.Scale(
    window,
    from_=0,
    to=100,
    orient=tkin.HORIZONTAL,
    label="Bathroom Intensity (%)",
    command=change_intensity,
    length=300
)
bathroom_intensity.set(50)
bathroom_intensity.pack(pady=10)


# Closet option.
closet_button = tkin.Radiobutton(
    window,
    text="Closet",
    variable=room,
    value="Closet",
    command=change_room,
    font=("Arial", 14)
)
closet_button.pack(pady=5)

# Closet brightness control.
closet_intensity = tkin.Scale(
    window,
    from_=0,
    to=100,
    orient=tkin.HORIZONTAL,
    label="Closet Intensity (%)",
    command=change_intensity,
    length=300
)
closet_intensity.set(50)
closet_intensity.pack(pady=10)


# Button to close the program.
exit_button = tkin.Button(
    window,
    text="Exit",
    command=exit_gui,
    font=("Arial", 14),
    width=12
)
exit_button.pack(pady=20)


# Keep the GUI running.
try:
    window.mainloop()

# Reset the GPIO pins when the program ends.
finally:
    GPIO.cleanup()