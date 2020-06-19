(function () {
    "use strict";

    utils.changeState(utils.ApplicationStates.manual);
    clearInterval(utils.queryValueIntervalId);

    $("#control-motor-left").on("click", async function () {
        const $this = $(this);
        $this.removeClass("table-danger").addClass("table-success");
        setMotors($this.attr("id"), true);

        await utils.sleep(utils.impulsDuration);

        $this.removeClass("table-success")
            .addClass("table-danger");
        setMotors($this.attr("id"), false);
    });

    $("#control-motor-right").on("click", async function () {
        const $this = $(this);
        $this.removeClass("table-danger").addClass("table-success");
        setMotors($this.attr("id"), true);

        await utils.sleep(utils.impulsDuration);

        $this.removeClass("table-success")
            .addClass("table-danger");
        setMotors($this.attr("id"), false);
    });

    $("#control-motor-up").on("click", async function () {
        const $this = $(this);
        $this.removeClass("table-danger").addClass("table-success");
        setMotors($this.attr("id"), true);

        await utils.sleep(utils.impulsDuration);

        $this.removeClass("table-success")
            .addClass("table-danger");
        setMotors($this.attr("id"), false);
    });

    $("#control-motor-down").on("click", async function () {
        const $this = $(this);
        $this.removeClass("table-danger").addClass("table-success");
        setMotors($this.attr("id"), true);

        await utils.sleep(utils.impulsDuration);

        $this.removeClass("table-success")
            .addClass("table-danger");
        setMotors($this.attr("id"), false);
    });

    function setMotors(motor, state) {
        let motorState = (state ? utils.motorStateOn : utils.motorStateOff);
        let url = "/set-motor/" + motor + "/" + motorState;
        $.ajax({
            url: url,
            type: 'GET',
            error: function (error) {
                console.log(error);
            }
        });
    }

})();
