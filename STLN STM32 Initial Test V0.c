/* STLN Initial Test System */

// Written for STM32 (device to be selected)
// N. Rupert 03/28/22

/* Pseudo Code
	
	1) Initialize Everything (mostly done by CubeIDE/CubeMX)
		- ADC Channel
		- UART
		- uint32_t HEAT_COUNT = 0x00000000
	
	2) Infinite While Loop
		
		a) Read ADC Value
			- Vout = 3.3V*(ADC_VAL/4096)
			- 30kOhm Thermisitor in voltage divider with 500kOhm Resistor
			- 3.3V Bias Voltage
			- R1 = 500k Resistor, R2 = 30k Thermisitor, Vout = 3.3V(Rtherm/(500k + Rtherm))
			- Rtherm = ((ADC_VAL/4096)*500kOhm)/(1+(ADC_VAL/4096))
	
		b) Compare ADC Value to Threshold Value
			- Set threshold at 0C for initial test
			- Rtherm = 94.98kOhm @ 0C
			- ADC Value = 961 @ 0C
				- If ADC Value < 961, turn on Heater and add 1 to count
				- Else turn off heater
			
		c) Print Info
			- Checking Temp...
				- Temp > 0C
					- Heater OFF
				- Temp < 0C
					- Heater ON
			- HEAT_COUNT
		
		d) Sleep for 5 seconds

*/

#define setTempC 0; // Temperature to stay above in degrees C
#define testRunTime 24; // number of hours to run the test for


// Initialization


	// "A bunch of peripheral initialization code" to be written
	
	// Variables used
	uint32_t cycleCount = 0x00000000; 		// Number of times (cycles) the while loop has run (each cycle is ~ 5s), initialized to 0x00000000
	uint32_t heatCount = 0x00000000; 		// Number of cycles the heater has been on (multiply this by 5s to get heater on time), initialized to 0x00000000
	uint32_t adcValue = 0x00000000; 		// Value read by ADC, initialized to 0x00000000
	
	
	float setTempK = (float)setTemp + 273.15;
	
	// Compute the number of cycles for the set test run time
	uint32_t numCycles = testRunTime*60*12;

	

// While Loop

	while(1){
		
		if(cycleCount < numCycles)
			{
		
			// Add 1 to cycleCount
				++cycleCount;
				
			// Read ADC Value
				HAL_ADC_Start(&hadc1);
				if(HAL_ADC_PollForConversion(&hadc1, 5)==HAL_OK)
					{
						adcValue = HAL_ADC_GetValue(&hadc1);
					}
				HAL_ADC_Stop(&hadc1);
			
			// Print
				// Checking Temperature...
			
			// Compare
				if(adcValue < 961)
					{
						// Set Heater Enable GPIO High
						// Set Heater Status LED GPIO High
						// Add 1 to heatCount
						++heatCount;
						// Print
							// Temperature is below 0C
							// Heater is ON
					}
				else
					{
						// Set Heater Enable GPIO Low
						// Set Heater Status LED GPIO Low
						// Print
							// Temperature is Above 0C
							// Heater is OFF
					
					}
			// Print
				// Each Cyle is 5 Seconds...
					// The heater has been on for (heatCount) of (cycleCount) total cycles
					
			// Store to SD Card
				// ADC_VAL,heatCount,cycleCount,numCycles,setTempC
				
			// Sleep
				HAL_Delay(5000); // Probably a better way to do long sleep times... this means the processor can not do anything else for 5 seconds
			}
			
			else
			{
				// Print Test Complete
					// Experiment ran for "cycleCount" cycles or "testRunTime" hours
					// of that time the heater was on for "heatCount" cycles to maintain a temperature above "setTempC" C
				
				// Store to SD Card
					// Test Complete
			}
		
		
	}

