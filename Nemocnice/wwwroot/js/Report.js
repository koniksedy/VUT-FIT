$("#Diagnosis1").autocomplete({
    source: function (request, response) {
        $.ajax({
            url: '/Doctor/GetAllDiagnosis',
            dataType: "json",
            data: {
                search: $("#Diagnosis1").val()
            },
            success: function (data) {
                response($.map(data, function (item) {
                    return {
                        label: "[".concat(pad(item.diagnosisId, 3)).concat("] ").concat(item.name),
                        value: pad(item.diagnosisId, 4)
                    }
                }));
            },
            error: function () {
                alert("Nepodařilo se získat diagnozy.");
            }
        });
    },
});

$("#Diagnosis2").autocomplete({
    source: function (request, response) {
        $.ajax({
            url: '/Doctor/GetAllDiagnosis',
            dataType: "json",
            data: {
                search: $("#Diagnosis2").val()
            },
            success: function (data) {
                response($.map(data, function (item) {
                    return {
                        label: "[".concat(pad(item.diagnosisId, 3)).concat("] ").concat(item.name),
                        value: pad(item.diagnosisId, 4)
                    }
                }));
            },
            error: function () {
                alert("Nepodařilo se získat diagnozy.");
            }
        });
    },
});

$("#Diagnosis3").autocomplete({
    source: function (request, response) {
        $.ajax({
            url: '/Doctor/GetAllDiagnosis',
            dataType: "json",
            data: {
                search: $("#Diagnosis3").val()
            },
            success: function (data) {
                response($.map(data, function (item) {
                    return {
                        label: "[".concat(pad(item.diagnosisId, 3)).concat("] ").concat(item.name),
                        value: pad(item.diagnosisId, 4)
                    }
                }));
            },
            error: function () {
                alert("Nepodařilo se získat diagnozy.");
            }
        });
    },
});

$("#Diagnosis4").autocomplete({
    source: function (request, response) {
        $.ajax({
            url: '/Doctor/GetAllDiagnosis',
            dataType: "json",
            data: {
                search: $("#Diagnosis4").val()
            },
            success: function (data) {
                response($.map(data, function (item) {
                    return {
                        label: "[".concat(pad(item.diagnosisId, 3)).concat("] ").concat(item.name),
                        value: pad(item.diagnosisId, 4)
                    }
                }));
            },
            error: function () {
                alert("Nepodařilo se získat diagnozy.");
            }
        });
    },
});

function pad(number, length) {

    var str = '' + number;
    while (str.length < length) {
        str = '0' + str;
    }

    return str;

}

function cured_button(button, name) {
    if (button.value == "false") {
        button.className = "btn btn-success";
        button.value = "true";
        $(name).value = "cured";
    } else {
        button.className = "btn btn-dark";
        button.value = "false";
        $(name).value = "noncured";
    }
}