// Please see documentation at https://docs.microsoft.com/aspnet/core/client-side/bundling-and-minification
// for details on configuring this project to bundle and minify static web assets.




function upravitBtnFunction() {
    document.getElementById("ReportText").disabled = false;
        document.getElementById("ReportText").readOnly = false;
        document.getElementById("RepareReportSaveBtn").disabled = false;
}

function unlockPatientInfo() {
    document.getElementById("UpdateName").disabled = false;
    document.getElementById("UpdateName").readOnly = false;

    document.getElementById("UpdateSurname").disabled = false;
    document.getElementById("UpdateSurname").readOnly = false;

    document.getElementById("UpdateTitle").disabled = false;
    document.getElementById("UpdateTitle").readOnly = false;

    document.getElementById("RC").disabled = false;
    document.getElementById("RC").readOnly = false;

    document.getElementById("UpdateInsurance").disabled = false;
    document.getElementById("UpdateInsurance").readOnly = false;

    document.getElementById("UpdateTel").disabled = false;
    document.getElementById("UpdateTel").readOnly = false;

    document.getElementById("UpdateEmail").disabled = false;
    document.getElementById("UpdateEmail").readOnly = false;

    document.getElementById("UpdateStreet").disabled = false;
    document.getElementById("UpdateStreet").readOnly = false;

    document.getElementById("UpdateHouseNum").disabled = false;
    document.getElementById("UpdateHouseNum").readOnly = false;

    document.getElementById("UpdateCity").disabled = false;
    document.getElementById("UpdateCity").readOnly = false;

    document.getElementById("UpdateZip").disabled = false;
    document.getElementById("UpdateZip").readOnly = false;

    document.getElementById("UpdateButton").disabled = false;
    document.getElementById("UpdateButton").readOnly = false;
}


function redirectToAction(hr) {
    window.location.href = hr;
}

function validateSSN() {

    if (!testRC('RC')) {
        return false;
    }

    if ($.ajax({
        url: '/Doctor/TestSocialSecurityNumUnique',
        dataType: "json",
        async: false,
        data: {
            num: document.getElementById("RC").value,
        },
        success: function (data) {
            return data;
        },
        error: function () {
            alert("Nepodařilo zkontrolovat rodné číslo.");
        }
    }).responseJSON === false) {
        alert("Pacient se zadaným rodným číslem již existuje.")
        return false;
    }

    return true;

    
}

function validateSSN_Admin_edit(id) {

    if (!testRC(id)) {
        return false;
    }

    if ($.ajax({
        url: '/Admin/TestSocialSecurityNumUnique',
        dataType: "json",
        async: false,
        data: {
            num: document.getElementById(id).value,
        },
        success: function (data) {
            return data;
        },
        error: function () {
            alert("Nepodařilo zkontrolovat rodné číslo.");
        }
    }).responseJSON === false) {
        alert("Pacient se zadaným rodným číslem již existuje.")
        return false;
    }

    return true;
}

// Kod převzat z https://www.zizka.ch/pages/programming/ruzne/rodne-cislo-identifikacni-cislo-rc-ico-kontrola-validace.html
function testRC(RC) {
    x = document.getElementById(RC).value;
    x = x.replace('/', '');
    try {
        if (x.length < 9) throw 1;
        var year = parseInt(x.substr(0, 2), 10);
        var month = parseInt(x.substr(2, 2), 10);
        var day = parseInt(x.substr(4, 2), 10);
        // Rodná čáslo před rokem 54 nepodléhají kontrole dělitelnosti.
        if ((x.length == 9) && (year < 54)) {
            // před rokem 54 bylo k měsícům povolemo přičítat pouze 50
            if ((month > 12 && month < 51) || (month > 62)) {
                alert("K měsícům rodných čísel před rokem 2003 je možné přičítat pouze 50.");
                throw 1;
            }
            return true;
        }
        var c = 0;
        if (x.length == 10) c = parseInt(x.substr(9, 1));
        var m = parseInt(x.substr(0, 9)) % 11;
        // Velkou vyjímku tvoří čísla se zbytkem dělitoelnosti 10.
        // U těchto čísel je jako kontrolní číslice zvolena 0
        if (m == 10) m = 0;
        // Pokud není dělitelné 11 bezezbytku, nebo se nejedná o vyjímku,
        // Pak je rodné číslo chybné.
        if (m != c) {
            alert("Rodné číslo nesplňuje kontrolu dělitelnosti 11.");
            throw 1;
        }
        // Čísla o délce 10, jsou rodná čísla od roku 1954.
        year += (year < 54) ? 2000 : 1900;
        // Zvláštní pravidla pro nová rodná čísla on roku 2003.
        if ((month > 70) && (year > 2003)) month -= 70;
        else if (month > 50) month -= 50;
        else if ((month > 20) && (year > 2003)) month -= 20;
        // Kontrola měsíců a dnů
        if (month == 0) {
            alert("Měsic zakódovaný v rodném čísle nemůže být 0.");
            throw 1;
        }
        if (month > 12) {
            alert("Měsíc zakódovaný v rodném čísle nemůže být větší než 12.");
            throw 1;
        }
        if (day == 0) {
            alert("Den zakódovaný v rodném čísle nemůže být roven 0.");
            throw 1;
        }
        if (day > 31) {
            alert("Den zakódovaný v rodném čísle nemůže být větší než 31.");
            throw 1;
        }
        // Kontrola, zda se jedná o datum v minulosti
        var dateFromRC = new Date(year, month-1, day);
        var dateNow = new Date();
        if (dateFromRC > dateNow) {
            alert("Datum zakódované v rodném čísle je datum z budoucnosti.");
            throw 1;
        }
    }
    catch (e) {
        return false;
    }
    return true;
}


function validateSSN_Admin() {

    if (!testRC('RC')) {
        return false;
    }

    if ($.ajax({
        url: '/Admin/TestSocialSecurityNumUnique',
        dataType: "json",
        async: false,
        data: {
            num: document.getElementById("RC").value,
        },
        success: function (data) {
            return data;
        },
        error: function () {
            alert("Nepodařilo zkontrolovat rodné číslo.");
        }
    }).responseJSON === false) {
        alert("Pacient se zadaným rodným číslem již existuje.")
        return false;
    }

    return true;
}

