def calculate_dconf(min_vals, max_vals):
    # Constants
    SAFE_GAIN = 25.0
    TARGET_OUTPUT = 100.0
    
    # 1. FIX THE "15" ISSUE: Add a small safety margin (Dead Zone)
    # This acts as a "floor". 0.02 is small enough to be unnoticeable, 
    # but large enough to eat ADC noise at the bottom.
    SAFETY_MARGIN = 0.02 

    # --- PHASE 1: NORMALIZE MEASUREMENTS ---
    # We convert raw ADC values (0-255 approx) into "Gain 1.0" domain (Volts/Ratio)
    norm_mins = [m / SAFE_GAIN for m in min_vals]
    norm_maxs = [m / SAFE_GAIN for m in max_vals]

    # --- PHASE 2: CALCULATE TOTAL REQUIREMENTS ---
    # For each slider, what exact Offset and Gain would make it perfect?
    req_total_gains = []
    req_total_offsets = []

    for i in range(5):
        # Range in normalized domain
        n_range = norm_maxs[i] - norm_mins[i]
        
        # Total Gain needed to turn this range into 100
        # Formula: Output = Range * TotalGain
        t_gain = TARGET_OUTPUT / n_range
        req_total_gains.append(t_gain)
        
        # Total Offset needed to make the bottom 0
        # We add the Safety Margin here.
        # If min is 0.32, we say offset is 0.34. 
        # Result: 0.32 - 0.34 = -0.02 (Clamped to 0)
        t_offset = norm_mins[i] + SAFETY_MARGIN
        req_total_offsets.append(t_offset)

    # --- PHASE 3: DISTRIBUTE GLOBAL VS LOCAL ---
    
    # Global Gain (Integer) - Average of all needs
    avg_gain = sum(req_total_gains) / len(req_total_gains)
    global_gain_int = int(round(avg_gain))
    
    # Global Offset (Float) - Average of all needs
    # (We use average to keep local offsets balanced around 0)
    avg_offset = sum(req_total_offsets) / len(req_total_offsets)
    global_offset_float = avg_offset

    # --- PHASE 4: CALCULATE FINE TUNING & FORMAT JSON ---
    
    cal_list = []
    
    for i in range(5):
        # Local Gain: The ratio between what we NEED and the GLOBAL setting
        local_gain = req_total_gains[i] / global_gain_int
        
        # Local Offset: The difference between what we NEED and the GLOBAL setting
        local_offset = req_total_offsets[i] - global_offset_float
        
        # Create the dictionary for this slider
        # We use f-strings to format exactly like your example:
        # +0.000 for offsets, 1.00 for gains
        slider_data = {
            f"{i+1}OFF": f"{local_offset:+.3f}",
            f"{i+1}G":   f"{local_gain:.2f}"
        }
        cal_list.append(slider_data)

    # Construct the final payload
    payload = {
        "cal": cal_list,
        "gain": f"{global_gain_int}", # Though your JSON example showed "1.09"? Assuming int based on previous prompt.
        "offset": f"{global_offset_float:.3f}",
        "device": "2025118H1D9" # Placeholder
    }
    
    return payload

# --- TEST WITH YOUR VALUES ---
my_minimals = [8, 8, 9, 8, 10]
my_maximals = [63, 63, 63, 63, 63]

result = calculate_dconf(my_minimals, my_maximals)

# Pretty print the JSON
print(result)