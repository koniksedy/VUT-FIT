using Microsoft.EntityFrameworkCore.Migrations;

namespace Nemocnice.Migrations
{
    public partial class InitialCreate : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "Tables1",
                columns: table => new
                {
                    Table1Id = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    ItemVal_Tab1 = table.Column<int>(nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Tables1", x => x.Table1Id);
                });

            migrationBuilder.CreateTable(
                name: "Tables2",
                columns: table => new
                {
                    Table2Id = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    ItemVal_Tab2 = table.Column<int>(nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Tables2", x => x.Table2Id);
                });
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "Tables1");

            migrationBuilder.DropTable(
                name: "Tables2");
        }
    }
}
