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
                        label: "[".concat(pad(item.diagnosisId, 4)).concat("] ").concat(item.name),
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
                        label: "[".concat(pad(item.diagnosisId, 4)).concat("] ").concat(item.name),
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
                        label: "[".concat(pad(item.diagnosisId, 4)).concat("] ").concat(item.name),
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
                        label: "[".concat(pad(item.diagnosisId, 4)).concat("] ").concat(item.name),
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

$("#Bill").autocomplete({
    source: function (request, response) {
        $.ajax({
            url: '/Doctor/GetAllActivities',
            dataType: "json",
            data: {
                search: $("#Bill").val()
            },
            success: function (data) {
                response($.map(data, function (item) {
                    return {
                        label: "[".concat(pad(item.id, 4)).concat("] ").concat(item.name),
                        value: pad(item.id, 4)
                    }
                }));
            },
            error: function () {
                alert("Nepodařilo se získat výkony.");
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
        document.getElementById(name).value = "cured";
    } else {
        button.className = "btn btn-dark";
        button.value = "false";
        document.getElementById(name).value = "noncured";
    }
}


function ReportValidation() {
    var Diag1 = document.getElementById("Diagnosis1").value;
    var Diag2 = document.getElementById("Diagnosis2").value;
    var Diag3 = document.getElementById("Diagnosis3").value;
    var Diag4 = document.getElementById("Diagnosis4").value;
    var Bill = document.getElementById("Bill").value;
    var Text = document.getElementById("ReportText").value;

    if (Text == "") {
        if (!confirm("Jste si jistí, že chcete uložit prázdnou zprávu?")) {
            stop_spinner();
            return false;
        }
    }

    if (Diag1 == "") {
        alert("Hlavní diagnóza musí být zadaná.");
        stop_spinner();
        return false;
    }
    if (Bill == "") {
        alert("Výkon musí být zadán.");
        stop_spinner();
        return false;
    }

    // Kontrola jednotlivých diagnoz
    result = $.ajax({
                url: '/Doctor/CheckNewReport',
                async: false,
                dataType: "json",
                data: {
                    diag1: Diag1,
                    diag2: Diag2,
                    diag3: Diag3,
                    diag4: Diag4,
                    activity: Bill
                },
                success: function (data) {
                    return data;
                },
                error: function () {
                    alert("Nepodařilo se zkontrolovat diagnozy a výkon.");
        }
    }).responseJSON;

    if (result == 1) {
        alert("Chybná hlavní diagnóza.\nDiagnóza s kódem " + Diag1 + " neexistuje.\nPro vyhledání všech diagnóz zadejte do pole diagnozy '?'");
    }
    else if (result == 2) {
        alert("Chybná druhá diagnóza.\nDiagnóza s kódem " + Diag2 + " neexistuje.\nPro vyhledání všech diagnóz zadejte do pole diagnozy '?'");
    }
    else if (result == 3) {
        alert("Chybná třetí diagnóza.\nDiagnóza s kódem " + Diag3 + " neexistuje.\nPro vyhledání všech diagnóz zadejte do pole diagnozy '?'");
    }
    else if (result == 4) {
        alert("Chybná čtvrtá diagnóza.\nDiagnóza s kódem " + Diag4 + " neexistuje.\nPro vyhledání všech diagnóz zadejte do pole diagnozy '?'");
    }
    else if (result == 10) {
        alert("Nezadávejte více stejných diagnóz.")
    }
    else if (result == 5) {
        alert("Chybný výkon.\nVýkon s kódem " + Bill + " neexistuje.\nPro vyhledání všech výkonů zadejte do pole výkonu '?'");
    }

    if (result != 0) {
        stop_spinner();
        return false;
    }

    return true;
}

function stop_spinner() {
    document.getElementById("saveButton").innerHTML = '<i style="margin-right: 3px" class="fa fa-floppy-o" aria-hidden="true"></i>';
}
