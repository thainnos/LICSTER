$(document).ready(async function () {
    "use strict";

    utils.changeState(utils.ApplicationStates.automatic);
    clearInterval(utils.queryValueIntervalId);

    function setSignal(condition, key) {
        $("#" + key).remove();
        const hmiElement = HmiMap[key];
        const position = hmiElement.position;
        let color = condition ? "yellow" : "green";

        let $svg = hmiElement.svg(color).attr("id", key);

        $svg.css("position", "absolute")
            .css("left", position.left)
            .css("top", position.top);

        $svg.insertAfter("#testbed-model");
    }


    function createArrowSvg(color, rotation) {
        return $("<svg>").html(
            '<svg width="45px" height="45px"> \
                <defs/><g><path d="M -0.5 20 L -0.5 10 L 19.5 10 L 19.5 -0.5 L 38.5 15 L 19.5 30.5 L 19.5 20 Z" fill="' + color + '" stroke="black" stroke-linejoin="round" stroke-miterlimit="10" pointer-events="none" transform="rotate(' + rotation + ' 20 15)"/></g> \
        </svg>'
        );
    }

    function createCircleSvg(color) {
        return $("<svg>").html(
            '<svg width="20" height="20"> \
                    <circle cx="10" cy="10" r="10" stroke="black" stroke-width="1" fill="' + color + '" /> \
            </svg>'
        );
    }

    function createArrowLeftSvg(color) {
        return createArrowSvg(color, 0);
    }

    function createArrowDownSvg(color) {
        return createArrowSvg(color, 90);
    }

    function createArrowRightSvg(color) {
        return createArrowSvg(color, 180);
    }

    function createArrowUpSvg(color) {
        return createArrowSvg(color, 270);
    }


    const HmiMap = {
        'topSensor': {'position': {'top': '10px', 'left': '684px'}, 'svg': createCircleSvg},
        'bottomSensor': {'position': {'top': '115px', 'left': '684px'}, 'svg': createCircleSvg},
        'leftSensor': {'position': {'top': '240px', 'left': '50px'}, 'svg': createCircleSvg},
        'rightSensor': {'position': {'top': '240px', 'left': '620px'}, 'svg': createCircleSvg},
        'motorUp': {'position': {'top': '145px', 'left': '730px'}, 'svg': createArrowUpSvg},
        'motorDown': {'position': {'top': '230px', 'left': '730px'}, 'svg': createArrowDownSvg},
        'motorLeft': {'position': {'top': '295px', 'left': '120px'}, 'svg': createArrowLeftSvg},
        'motorRight': {'position': {'top': '295px', 'left': '650px'}, 'svg': createArrowRightSvg}
    };


    function setHmi(values) {
        for (let key in HmiMap) {
            if (HmiMap.hasOwnProperty(key)) {
                setSignal(values[key] === "true", key);
            }
        }
    }

    function queryValues() {
        $.ajax({
            url: '/process/values',
            type: 'GET',
            success: function (values) {
                setHmi(values);
            },
            error: function (error) {
                console.log(error);
            }
        });
    }

    utils.queryValueIntervalId = setInterval(queryValues, utils.queryProcessValuesInterval);
});