// Please see documentation at https://docs.microsoft.com/aspnet/core/client-side/bundling-and-minification
// for details on configuring this project to bundle and minify static web assets.

// HACK původně mělo zajišťovat funkčnost Z kartotéky Nový pacient. Pokud nefunguje, přidat zpět.
/*
    $('#exampleModal').on('show.bs.modal', function (event) {
        var button = $(event.relatedTarget) // Button that triggered the modal
})
*/



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

    document.getElementById("UpdateNum").disabled = false;
    document.getElementById("UpdateNum").readOnly = false;

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

// add row
$("#addRowReport").click(function () {
    var html = '';
    html += '<div id="inputFormRowReport">';
    html += '<div class="input-group mb-3">';
    html += '<input type="text" name="Diagnosis[]" class="form-control m-input" placeholder="Diagnoza" autocomplete="off">';
    html += '<div class="input-group-append">';
    html += '<button id="removeRowReport" type="button" class="btn btn-danger">Smazat</button>';
    html += '<div>';
    html += '<button name="curedButton[]" type="button" class="btn btn-dark" onclick="curedFunction(this)" value="false">Vyléčeno</button>';
    html += '<input type="hidden" name="DiagnosisState[]" value="noncured" />';
    html += '</div>';
    html += '</div>';
    html += '</div>';

    $('#newRow').append(html);
});

// remove row
$(document).on('click', '#removeRowReport', function () {
            $(this).closest('#inputFormRowReport').remove();
});


function curedFunction(thatButton) {
    if (thatButton.value == "false") {
        thatButton.className = "btn btn-success";
        thatButton.value = "true";
        thatButton.nextElementSibling.value = "cured";
    } else {
        thatButton.className = "btn btn-dark";
        thatButton.value = "false";
        thatButton.nextElementSibling.value = "noncured";
    }
}

function redirectToAction(hr) {
    window.location.href = hr;
}


// add row
$("#addRowCheckup").click(function () {
    var html = '';
    html += '<div id="inputFormRowCheckup">';
    html += '<div class="input-group mb-3">';
    html += '<input type="text" name="Diagnosis[]" class="form-control m-input" placeholder="Diagnoza" autocomplete="off">';
    html += '<div class="input-group-append">';
    html += '<button id="removeRowCheckup" type="button" class="btn btn-danger">Smazat</button>';
    html += '</div>';
    html += '</div>';

    $('#newRow').append(html);
});

// remove row
$(document).on('click', '#removeRowCheckup', function () {
    $(this).closest('#inputFormRowCheckup').remove();
});