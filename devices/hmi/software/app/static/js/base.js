const utils = {};

(function () {
        'use strict';

        clearInterval(utils.queryStateIntervalId);
        clearInterval(utils.queryApplicationStateIntervalId);

        function updateHmi() {
            const $state = $("#application-state");
            $state.text(utils.applicationState.text);
            $state.attr("class", utils.applicationState.classes.join(" "))
        }

        $("#application-state").on("click", function () {
            reset();
            getOrSetApplicationState(utils.applicationState);
        });

        $("#stop-button").on("click", function () {
            const state = utils.applicationState;
            getOrSetApplicationState(utils.ApplicationStates.stop);
            sleep(100);
            getOrSetApplicationState(state);
        });

        function sleep(ms) {
            return new Promise(resolve => setTimeout(resolve, ms));
        }

        function updateProcessState() {
            const $processState = $("#process-state");
            $processState.text(utils.processState.text);
            $processState.attr("class", utils.processState.classes.join(" "))
        }

        function queryProcessState() {
            $.ajax({
                url: '/process/state',
                type: 'GET',
                dataType: 'JSON',
                success: function (state) {
                    utils.processState = state;
                    updateProcessState();
                },
                error: function (error) {
                    console.log(error);
                }
            });
        }

        function reset() {
            $.ajax({
                url: '/application/reset',
                type: 'GET',
                dataType: 'JSON',
                error: function (error) {
                    console.log(error);
                }
            });
        }

        function getOrSetApplicationState(state) {
            let query = "";
            if (typeof (state) !== 'undefined') {
                query = "/" + state;
            }
            $.ajax({
                url: '/application/state' + query,
                type: 'GET',
                dataType: 'JSON',
                success: function (state) {
                    utils.applicationState = state;
                    updateHmi();
                },
                error: function (error) {
                    console.log(error);
                }
            });
        }

        getOrSetApplicationState();

        utils.ApplicationStates = {
            "automatic": "automatic",
            "manual": "manual",
            "stop": "stop",
            "reset": "reset"
        };
        Object.freeze(utils.ApplicationStates);
        utils.changeState = getOrSetApplicationState;
        utils.sleep = sleep;
        utils.impulsDuration = 500;
        utils.queryProcessStateInterval = 50;
        utils.queryProcessValuesInterval = 50;
        utils.queryApplicationStateInterval = 250;
        utils.applicationState = "";
        utils.processState = "";
        utils.motorStateOn = 1;
        utils.motorStateOff = 0;

        utils.queryStateIntervalId = setInterval(queryProcessState, utils.queryProcessStateInterval);
        utils.queryApplicationStateIntervalId = setInterval(getOrSetApplicationState, utils.queryApplicationStateInterval);

    }
)();

