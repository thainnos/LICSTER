"""
Tests if the possible States are initialized correctly.
"""

import states.application as app_states
import states.process as proc_states
import states.singleton as singleton


class TestApplicationStates():
    """
    This class contains all methods for testing if the
    states.application classes are initialized correctly.
    """
    def test_automatic_state(self):
        AutomaticState = app_states.AutomaticState()
        assert AutomaticState.modbus_value == 0
        assert AutomaticState.text == "Automatic"
        assert AutomaticState.name == "automatic"
        assert AutomaticState.classes == ["btn btn-success mr-auto"]

    def test_manual_state(self):
        ManualState = app_states.ManualState()
        assert ManualState.modbus_value == 1
        assert ManualState.text == "Manual"
        assert ManualState.name == "manual"
        assert ManualState.classes == ["btn btn-warning mr-auto"]

    def test_stop_state(self):
        StopState = app_states.StopState()
        assert StopState.modbus_value == 2
        assert StopState.text == "STOPPED"
        assert StopState.name == "stop"
        assert StopState.classes == ["btn btn-danger mr-auto"]

    def test_disconnected(self):
        DisconnectedState = app_states.Disconnected()
        assert DisconnectedState.modbus_value == 3
        assert DisconnectedState.text == "DISCONNECTED"
        assert DisconnectedState.name == "disconnected"
        assert DisconnectedState.classes == ["btn btn-info mr-auto"]

    def test_application_state_map(self):
        AppStateMap = app_states.ApplicationStateMap()
        keyList = ['automatic', 0, 'manual', 1, 'stop', 2, 'disconnected', 3]
        for key in keyList:
            assert key in AppStateMap.states


class TestProcessStates():
    """
    This class contains all methods for testing if the
    states.process classes are initialized correctly.
    """
    def test_pending_state(self):
        PendingState = proc_states.PendingState()
        assert PendingState.name == "pending"
        assert PendingState.text == "Pending"
        assert PendingState.modbus_value == 0
        assert PendingState.classes == ["btn btn-success mr-auto"]

    def test_towards_punching_state(self):
        TowardsState = proc_states.TowardsPunchingMachineState()
        assert TowardsState.name == "to-punch"
        assert TowardsState.text == "Move Right"
        assert TowardsState.modbus_value == 1
        assert TowardsState.classes == ["btn btn-warning mr-auto"]

    def test_punching_down_state(self):
        PunchingMachineDown = proc_states.PunchingMachineDownState()
        assert PunchingMachineDown.name == "punch-down"
        assert PunchingMachineDown.text == "Move Down"
        assert PunchingMachineDown.modbus_value == 2
        assert PunchingMachineDown.classes == ["btn btn-warning mr-auto"]

    def test_punching_up_state(self):
        PunchingMachineUp = proc_states.PunchingMachineUpState()
        assert PunchingMachineUp.name == "punch-up"
        assert PunchingMachineUp.text == "Move Up"
        assert PunchingMachineUp.modbus_value == 3
        assert PunchingMachineUp.classes == ["btn btn-warning mr-auto"]

    def test_from_punching_state(self):
        FromPunchingMachine = proc_states.FromPunchingMachineState()
        assert FromPunchingMachine.name == "from-punch"
        assert FromPunchingMachine.text == "Move Left"
        assert FromPunchingMachine.modbus_value == 4
        assert FromPunchingMachine.classes == ["btn btn-warning mr-auto"]

    def test_process_state_map(self):
        ProcStateMap = proc_states.ProcessStateMap()
        keyList = ['pending', 0, 'to-punch', 1, 'punch-down', 2, 'punch-up', 3,
                   'from-punch', 4]
        for key in keyList:
            assert key in ProcStateMap.states


class TestSingleton():
    """
    This class contains all methods for testing if the
    states.Singelton class is working correctly.
    """
    def test_singleton(self):
        """
        Tests if the singleton is working by checking
        if the references of instance_one and instance_two
        are pointing to the same object.
        """
        class SingletonInheriter(metaclass=singleton.Singleton):
            """ This class is needed to test the Singleton. """
            pass
        instance_one = SingletonInheriter()
        instance_two = SingletonInheriter()
        assert instance_one is instance_two
