/* Javascript pro zobrazení PatientProgile, které se stará o předávání zpráv jiným lékařům.
 * Autor: Michal Šedý <xsedym02>
 */

$("#NewDoctor").autocomplete({
    source: function (request, response) {
        $.ajax({
            url: '/Doctor/GetAllDoctors',
            dataType: "json",
            data: {
                search: $("#NewDoctor").val()
            },
            success: function (data) {
                response($.map(data, function (item) {
                    return {
                        label: item.name,
                        value: item.name,
                        icz: item.id
                    }
                }));
            },
            error: function () {
                alert("Nepodařilo se získat doktory.");
            }
        });
    },
    select: function (event, ui) {
        document.getElementById("doctorICZ").value = ui.item.icz;
    }
});