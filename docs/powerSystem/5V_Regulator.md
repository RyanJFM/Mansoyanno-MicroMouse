# 5V Voltage Regulation Circuit

![5V Regulator Schematic](../../../media/schematics/5v-regulator/5v-regulator.png)

## 1. Overview

It takes the battery's variable voltage and converts it into a stable 5V output.

## IC's behaviour

# Maintaining a consistent 5V

so basically this keeps the 5v output consistent even tho the battery voltage changes. 
ex:-
    Battery:8.4 v -> VIN:8.4 v -> LMZ21701: (VOUT: 5.0 v, FB: 0.5 v)

    when the battery level goes down 

    Battery:6.0 v -> VIN:6.0 v -> LMZ21701: (VOUT: 5.0 v, FB: 0.5 v)

so how does it know to keep 

But HOW does it know whether to increase or decrease the output?

This is where the feedback control loop comes in. The IC continuously compares the FB voltage against its internal 0.8V reference.

Imagine these situations:

| Actual FB Voltage | What the Regulator Understands | Action |
|:---:|---|---|
| 0.7V | Output voltage is too low | Increase energy delivered |
| 0.8V | Output voltage is at the target | Maintain regulation |
| 0.9V | Output voltage is too high | Reduce energy delivered |

Feedback controls the output voltage. Protection mechanisms determine when the regulator must stop operating.

# Input under voltage lockout (UVLO)

The regulator tries to compensate, but it has limits.

If it reaches those limits:
- It may no longer maintain 5V.
- The output voltage starts dropping.
- FB falls below 0.8V.
- The Power Good (PG) signal indicates that the output is not in regulation.

When does it actually shut down?
The LMZ21701 has protection mechanisms, including:
- UVLO: Input voltage is too low.
- Overcurrent protection: Excessive current is detected.
- Thermal shutdown: The IC becomes too hot.
- EN pin: An external signal can disable it.

# Enable (EN)

basically a ON/OFF

| EN voltage | Behaviour |
|---|---|
| HIGH (at least 0.9V) | Enabled |
| LOW (0.3V or less) | Disabled |

There's also an internal 400kΩ pull-down resistor. so its off by default but in our schematic its connected to VIN so .. EN is high

# Soft start (SS)

The regulator must raise its output from 0V to 5V. the time we have settled on is 1.42 ms

The soft-start time is calculated using:

$$
T_{SS} = \frac{C_{SS} \times 1.25V}{2.9\mu A}
$$

We use:

$$
C_{SS} = 3300pF
$$

Substituting the values:

$$
T_{SS} = \frac{3300 \times 10^{-12} \times 1.25}
{2.9 \times 10^{-6}}
$$

Therefore:

$$
\boxed{T_{SS} \approx 1.42ms}
$$

# Power Good (PG)

Suppose the regulator is enabled, but its output hasn't reached 5V yet.
How would another circuit know whether the supply is ready?
That's what PG is for.
PG pin is an open-drain output.

An open-drain output behaves somewhat like an electronic switch connected to ground.
It can pull the signal LOW, but it cannot independently drive it HIGH.

Therefore, we need an external pull-up resistor.
The selected pull-up resistor is:

$$
R_{PG}=10k\Omega
$$

When the output isn't ready, the internal switch pulls PG **LOW**.

When the output is ready, the switch releases the signal, allowing the resistor to pull PG **HIGH**.

The datasheet gives:

$$
R_{PG-MIN}=\frac{V_{PULL-UP}}{2mA}
$$

For a 5V pull-up:

$$
R_{PG-MIN}=\frac{5}{0.002}=2.5k\Omega
$$

10K ohms satisfy the need

# Output Voltage Setting (FB)

It uses the feedback pin FB to monitor a fraction of the output voltage.

$$
R_{FBT}=R_{FBB}
\left(\frac{V_{OUT}}{0.8}-1\right)
$$

Given:

$$
R_{FBT}=52.5k\Omega
$$

$$
R_{FBB}=10k\Omega
$$

Substituting into the output voltage equation:

$$
V_{OUT}=0.8\left(1+\frac{52.5k}{10k}\right)
$$

Therefore:

$$
\boxed{V_{OUT}=5V}
$$

# Thermal Protection

The recommended maximum junction temperature is 125°C.


## Component explanation

| Component | Value | Purpose | Explanation |
|---|---:|---|---|
| L5 | 4.7µH | External input filter | Works with the input capacitors to filter high-frequency noise and voltage fluctuations entering the regulator. |
| C_VBAT1 | 0.1µF | Input bypass | Filters high-frequency noise on the input supply. |
| C_VBAT2 | 0.1µF | Input bypass | Provides additional high-frequency noise filtering. |
| C_VBAT4 | 47µF | Input bulk capacitor | Provides a local reserve of charge and helps handle input voltage fluctuations during sudden changes in current demand. |
| C_VBAT3 | 0.1µF | Input bypass | Provides additional high-frequency noise filtering. |
| R_FBT1 | 52.5kΩ | Feedback divider | Upper resistor in the feedback divider. Helps determine the regulated output voltage. |
| R_FBB1 | 10kΩ | Feedback divider | Lower resistor in the feedback divider. Together with R_FBT1, sets the output voltage to 5V. |
| C_SS1 | 3300pF | Soft-start capacitor | Controls how quickly the output voltage rises when the regulator starts, reducing sudden current demand during startup. |
| R_PG1 | 10kΩ | Power Good pull-up | Pulls the PG signal HIGH when the regulator indicates that its output is within the regulation range. |
| C_5V4_4 | 47µF | Output bulk capacitor | Helps stabilize the 5V output and supplies charge during sudden increases in load current. |
| C_5V4_5 | 0.1µF | Output bypass capacitor | Filters high-frequency noise on the 5V output. |

## Additional stuff

# what is MGND here

In a micromouse, motors draw relatively large, rapidly changing currents. These currents can introduce electrical noise into the ground network.

if the motor current and sensitive electronics shared the exact same ground path:
- Motors draw a large current.
- The current flowing through PCB trace resistance creates small voltage drops.
- Those voltage changes can affect the MCU, sensors and IMU readings.

To reduce this problem, designers may separate the ground connections into different domains:
- GND: Ground for digital and general electronics.
- AGND: Analog ground for sensitive analog circuits.
- MGND: Motor ground for motor-related circuitry.

# what are these Splitters

The symbol represents a small PCB connection with two pads.

Why is this useful?

Motors and switching converters can generate electrical noise. If their return currents flow through the same PCB paths as sensitive analog electronics, they can introduce unwanted voltage fluctuations.
Separating ground domains and carefully choosing where they connect helps manage this problem

