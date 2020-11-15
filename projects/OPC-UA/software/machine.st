PROGRAM My_Program
    VAR
        (*
        Application states:
        switch(state):
            case 0: AUTOMATIC
            case 1: MANUAL
            case 2: EMERGENCY STOPP
            default: go into EMERGENCY STOPP
        
        Process states:
        switch(state):
            case 0: Waiting for orders to start execution
            case 1: Moving the conveyor belt towards punching machine
            case 2: Moving the punching machine down to punch good
            case 3: Moving the punching machine up
            case 4: Moving the conveyor belt away from the punching machine
            default: go into EMERGENCY STOP
         *)

        (* Punching machine moves UP *)
        output1 AT %MW0 : UINT := 0;

        (* Punching machine moves DOWN *)
        output2 AT %MW1 : UINT := 0;

        (* Converyer belt moves TO punching machine *)
        output3 AT %MW2 : UINT := 0;

        (* Converyer belt moves FROM punching machine *)
        output4 AT %MW3 : UINT := 0;

        (* Start light sensor of the convey belt*)
        input1 AT %MW4 : UINT := 0;

        (* Sensor at the convey belt near the punching machine *)
        input2 AT %MW5 : UINT := 0;

        (* Sensor punching machine lower limit switch *)
        input3 AT %MW6 : UINT := 0;

        (* Sensor punching machine upper limit switch *)
        input4 AT %MW7 : UINT := 0;

        (* IF OPC-UA is used, set this value *)
        opcua AT %MW8 : UINT := 0;
  
        (* The current application state *)
        hmi_application_state AT %MW10 : UINT := 0;

        (* The current process state *)
        process_state AT %MW11 : UINT := 0;

        (* The current amount of orders *)
        order_count AT %MW12 : UINT := 1;

        (* The reset trigger to get the PLC out of the EMERGENCY STOP *)
        reset_button AT %MW13 : UINT := 0;

        (* The current application state *)
        plc_application_state AT %MW14 : UINT := 0;


        (* PHYSICAL OUTPUTS *)
        (* Physical outputs of the punching machine going UP *)
        punch_machine_up AT %QX100.0 : BOOL := 0;

        (* Physical outputs of the punching machine going DOWN *)
        punch_machine_down AT %QX100.1 : BOOL := 0;

        (* Physical outputs of the conveyer belt moving TO the punching machine *)
        conveyer_belt_to_punch AT %QX100.2 : BOOL := 0;

        (* Physical outputs of the conveyer belt moving FROM the punching machine *)
        conveyer_belt_to_start AT %QX100.3 : BOOL := 0;


        (* PHYSICAL INPUTS *)
        (* Physical inputs the upper limit switch *)
        upper_limit_switch AT %IX100.0 : BOOL := 0;

        (* Physical inputs the lower limit switch *)
        lower_limit_switch AT %IX100.1 : BOOL := 0;

        (* Physical inputs from the EXIT light sensor. TRUE if there is nothing between the sensor, FALSE if there is something *)
        start_light_sensor AT %IX100.2 : BOOL := 0;

        (* Physical inputs from the punching machine light sensor. TRUE if there is nothing between the sensor, FALSE if there is something *)
        punch_light_sensor AT %IX100.3 : BOOL := 0;
    END_VAR

    VAR
        (* Last active state to go back to after leaving EMERGENCY STATE *)
        last_state : UINT := 0;

        (* Delay between processes *)
        delay : UINT := 0;

        (* Process timeout. *)
        timeout : UINT := 0;

        (* Timeout upper limit *)
        TIMEOUT_MAX : UINT := 100;
    END_VAR

    IF opcua < 1 THEN
        IF start_light_sensor = TRUE THEN
            input1 := 1;
        ELSE
            input1 := 0;
        END_IF;
        IF punch_light_sensor = TRUE THEN
            input2 := 1;
        ELSE
            input2 := 0;
        END_IF;
        IF lower_limit_switch = TRUE THEN
            input3 := 1;
        ELSE
            input3 := 0;
        END_IF;
        IF upper_limit_switch = TRUE THEN
            input4 := 1;
        ELSE
            input4 := 0;
        END_IF;
    END_IF;

    (* IF the state coming in from the HMI does not match the internal state of the PLC THEN *)
    IF plc_application_state <> hmi_application_state THEN
        (* IF the current state of the PLC is AUTOMATIC...*)
        IF plc_application_state = 0 THEN
            (* ... and the incoming state of the HMI is MANUAL... *)
            IF hmi_application_state = 1 THEN
                (* ... check if the process state is in PENDING. *)
                IF process_state = 0 THEN
                    (* If so, change the application state of the PLC according the HMI. Else do nothing. *)
                    plc_application_state := hmi_application_state;
                END_IF;
            (* ... and the incoming state of the HMI is EMERGENCY STOP... *)
            ELSIF hmi_application_state = 2 THEN
                (* ... change the state of the PLC at once according to the HMI.*)
                last_state := plc_application_state;
                plc_application_state := hmi_application_state;
                hmi_application_state := last_state;
            (* For any other state of the HMI, do nothing.*)
            END_IF;
        
        (* IF the current state of the PLC is MANUAL...*)
        ELSIF plc_application_state = 1 THEN
            (* ... and the incoming state of the HMI is AUTOMATIC... *)
            IF hmi_application_state = 0 THEN
                (* ... change the state of the PLC at once according to the HMI. *)
                plc_application_state := hmi_application_state;
            (* ... and the incoming state of the HMI is EMERGENCY STOP... *)
            ELSIF hmi_application_state = 2 THEN
                (* ... change the state of the PLC at once according to the HMI.*)
                last_state := plc_application_state;
                plc_application_state := hmi_application_state;
                hmi_application_state := last_state;
            (* For any other state of the HMI, do nothing.*)
            END_IF;

        (* IF the current state of the PLC is EMERGENCY STOP...*)
        ELSIF plc_application_state = 2 THEN
            (* IF the RESET trigger is sent... *)
            IF reset_button = 1 THEN
                (*... change the state of the PLC back to AUTOMATIC. *)
                plc_application_state := hmi_application_state;
                order_count := 0;
                reset_button := 0;
            END_IF;
        END_IF;
    END_IF;

    (* automatic. Wait on incoming order to start executing the process *)
    IF plc_application_state = 0 THEN

        (* In process_state 0 the machine stays idle and waits for orders *)
        IF process_state = 0 THEN
            output1 := 0;
            output2 := 0;
            output3 := 0;
            output4 := 0;

            (* If there is an order and the product on the conveyer belt is in the starting position, start producing *)
            IF delay > 100 AND input1 = 0 AND order_count > 0 THEN
                process_state := 1;
                timeout := 0;
            END_IF;

        (* In process_state 1 the conveyer belt is moving from the EXIT to the punching machine ... *)
        ELSIF process_state = 1 THEN
            output1 := 0;
            output2 := 0;
            output3 := 1;
            output4 := 0;

            (* ... until the punching machine light sensor is reached *)
            IF input2 = 0 THEN
                process_state := 2;
                timeout := 0;

            (* In case it never reaches the sensor in a certain time, stop and go into error mode *)
            ELSIF timeout > TIMEOUT_MAX THEN
                order_count := 0;
                process_state := 0;
                hmi_application_state := 2;
            END_IF;

        (* In process_state 2 the conveyer belt is stopped and the punching machine is gowing down ... *)
        ELSIF process_state = 2 THEN
            output1 := 0;
            output2 := 1;
            output3 := 0;
            output4 := 0;

            (* ... until the lower limit switch is reached *)
            IF input3 = 1 THEN
                process_state := 3;
                timeout := 0;

            (* In case it never reaches the lower limit switch in a certain time, stop and go into error mode *)
            ELSIF timeout > TIMEOUT_MAX THEN
                order_count := 0;
                process_state := 0;
                plc_application_state := 2;
            END_IF;

        (* In process_state 3 the punching machine is going up ... *)
        ELSIF process_state = 3 THEN
            output1 := 1;
            output2 := 0;
            output3 := 0;
            output4 := 0;

            (* ... until the upper limit switch is reached *)
            IF input4 = 1 THEN
                process_state := 4;
                timeout := 0;

            (* In case it never reaches the upper limit switch in a certain time, stop and go into error mode *)
            ELSIF timeout > TIMEOUT_MAX THEN
                order_count := 0;
                process_state := 0;
                plc_application_state := 2;
            END_IF;

        (* In process_state 4 the conveyer belt is moving from the punching machine to the EXIT ... *)
        ELSIF process_state = 4 THEN
            output1 := 0;
            output2 := 0;
            output3 := 0;
            output4 := 1;

            (* ... until the EXIT light sensor is reached. One order is now produced *)
            IF input1 = 0 THEN
                process_state := 0;
                delay := 0;
                timeout := 0;
                order_count := order_count -1;

            (* In case it never reaches the EXIT light sensor in a certain time, stop and go into error mode *)
            ELSIF timeout > TIMEOUT_MAX THEN
               order_count := 0;
               process_state := 0;
               plc_application_state := 2;
            END_IF;
        ELSE
           order_count := 0;
           process_state := 0;
           plc_application_state := 2;
        END_IF;

    (* Manual state. Receive dedicated movements for conveyer belt and punching machine *)
    ELSIF plc_application_state = 1 THEN
        plc_application_state := 1;

    (* If no state is selected, go into idle and do nothing *)
    (* Emergency stop. Do nothing until changed *)
    ELSE
        plc_application_state := 2;
        process_state := 0;
        delay := 0;
        timeout := 0;
        output1 := 0;
        output2 := 0;
        output3 := 0;
        output4 := 0;
    END_IF;


    (* Map the variables onto the actual outputs *)
    IF output1 = 1 THEN
        punch_machine_up := TRUE;
    ELSE
        punch_machine_up := FALSE;
    END_IF;
    IF output2 = 1 THEN
        punch_machine_down := TRUE;
    ELSE
        punch_machine_down := FALSE;
    END_IF;
    IF output3 = 1 THEN
        conveyer_belt_to_punch := TRUE;
    ELSE
        conveyer_belt_to_punch := FALSE;
    END_IF;
    IF output4 = 1 THEN
        conveyer_belt_to_start := TRUE;
    ELSE
        conveyer_belt_to_start := FALSE;
    END_IF;
  
    (* Increase the timer *)
    delay := delay + 1;
    timeout := timeout + 1;

END_PROGRAM


CONFIGURATION Config0
  RESOURCE Res0 ON PLC
    TASK TaskMain(INTERVAL := T#50ms,PRIORITY := 0);
    PROGRAM Inst0 WITH TaskMain : My_Program;
  END_RESOURCE
END_CONFIGURATION
