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

function kokotClick() {
    location.href = "/Home";
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

// TODO změnit hodnotu DiagnosisState
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
