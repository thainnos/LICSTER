(function () {

    utils.changeState(utils.ApplicationStates.automatic);
    clearInterval(utils.queryValueIntervalId);

    $("#decrease-order-value").on("click", function () {
        const $orderValue = $("#place-order-value");
        const val = $orderValue.val();
        const min = $orderValue.attr("min");
        if (Number(min) < Number(val)) {
            $orderValue.val(Number(val) - 1);
        }

    });

    $("#increase-order-value").on("click", function () {
        const $orderValue = $("#place-order-value");
        const val = $orderValue.val();
        const max = $orderValue.attr("max");
        if (Number(max) > Number(val)) {
            $orderValue.val(Number(val) + 1);
        }
    });


    $("#place-order-button").on("click", function () {
        const count = $("#place-order-value").val();
        getOrSetOrder(count);
    });

    function updateOrderValue(orderValue) {
        $("#order-value").text(orderValue);
        const condition = orderValue > 0;
        $(".disable-while-running").each(function () {
            $(this).attr("disabled", condition)
        });
    }


    function getOrSetOrder(count) {
        let query = "";
        if (typeof (count) !== 'undefined') {
            query = "/" + count;
        }
        $.ajax({
            url: '/orders' + query,
            type: 'GET',
            success: function (orderValue) {
                if (orderValue !== "") {
                    updateOrderValue(orderValue);
                }
            },
            error: function (error) {
                console.log(error);
            }
        });
    }

    setInterval(getOrSetOrder, 500);

})();
