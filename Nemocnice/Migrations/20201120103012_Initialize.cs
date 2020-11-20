using System;
using Microsoft.EntityFrameworkCore.Migrations;

namespace Nemocnice.Migrations
{
    public partial class Initialize : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "AddressT",
                columns: table => new
                {
                    AddressId = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    HouseNumber = table.Column<int>(nullable: false),
                    StreetName = table.Column<string>(nullable: true),
                    City = table.Column<string>(nullable: false),
                    ZIP = table.Column<int>(nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_AddressT", x => x.AddressId);
                });

            migrationBuilder.CreateTable(
                name: "AdminT",
                columns: table => new
                {
                    AdminId = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    UserId = table.Column<int>(nullable: false),
                    Role = table.Column<string>(nullable: true),
                    WorkPhone = table.Column<string>(nullable: false),
                    OutsorceCompany = table.Column<string>(nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_AdminT", x => x.AdminId);
                });

            migrationBuilder.CreateTable(
                name: "AspNetRoles",
                columns: table => new
                {
                    Id = table.Column<string>(nullable: false),
                    Name = table.Column<string>(maxLength: 256, nullable: true),
                    NormalizedName = table.Column<string>(maxLength: 256, nullable: true),
                    ConcurrencyStamp = table.Column<string>(nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_AspNetRoles", x => x.Id);
                });

            migrationBuilder.CreateTable(
                name: "AspNetUsers",
                columns: table => new
                {
                    Id = table.Column<string>(nullable: false),
                    UserName = table.Column<string>(maxLength: 256, nullable: true),
                    NormalizedUserName = table.Column<string>(maxLength: 256, nullable: true),
                    Email = table.Column<string>(maxLength: 256, nullable: true),
                    NormalizedEmail = table.Column<string>(maxLength: 256, nullable: true),
                    EmailConfirmed = table.Column<bool>(nullable: false),
                    PasswordHash = table.Column<string>(nullable: true),
                    SecurityStamp = table.Column<string>(nullable: true),
                    ConcurrencyStamp = table.Column<string>(nullable: true),
                    PhoneNumber = table.Column<string>(nullable: true),
                    PhoneNumberConfirmed = table.Column<bool>(nullable: false),
                    TwoFactorEnabled = table.Column<bool>(nullable: false),
                    LockoutEnd = table.Column<DateTimeOffset>(nullable: true),
                    LockoutEnabled = table.Column<bool>(nullable: false),
                    AccessFailedCount = table.Column<int>(nullable: false),
                    Discriminator = table.Column<string>(nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_AspNetUsers", x => x.Id);
                });

            migrationBuilder.CreateTable(
                name: "DiagnosisT",
                columns: table => new
                {
                    DiagnosisId = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Name = table.Column<string>(nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_DiagnosisT", x => x.DiagnosisId);
                });

            migrationBuilder.CreateTable(
                name: "DoctorT",
                columns: table => new
                {
                    DoctorId = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    ICZ = table.Column<int>(nullable: false),
                    UserId = table.Column<int>(nullable: false),
                    WorkPhone = table.Column<string>(nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_DoctorT", x => x.DoctorId);
                });

            migrationBuilder.CreateTable(
                name: "HealthConditionT",
                columns: table => new
                {
                    HealthConditionId = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    SocialSecurityNum = table.Column<string>(nullable: false),
                    Height = table.Column<int>(nullable: false),
                    Weight = table.Column<int>(nullable: false),
                    BloodType = table.Column<string>(nullable: true),
                    LastCheckupDate = table.Column<DateTime>(nullable: false),
                    Allergys = table.Column<string>(nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_HealthConditionT", x => x.HealthConditionId);
                });

            migrationBuilder.CreateTable(
                name: "InsureEmpT",
                columns: table => new
                {
                    PersonalId = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    UserId = table.Column<int>(nullable: false),
                    Possition = table.Column<string>(nullable: true),
                    WorkPhone = table.Column<string>(nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_InsureEmpT", x => x.PersonalId);
                });

            migrationBuilder.CreateTable(
                name: "MedicallActivityPriceT",
                columns: table => new
                {
                    MedicallActivityPriceId = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Name = table.Column<string>(nullable: false),
                    Amount = table.Column<decimal>(nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_MedicallActivityPriceT", x => x.MedicallActivityPriceId);
                });

            migrationBuilder.CreateTable(
                name: "UserT",
                columns: table => new
                {
                    UserId = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Login = table.Column<string>(nullable: false),
                    Name = table.Column<string>(nullable: false),
                    Surname = table.Column<string>(nullable: false),
                    Title = table.Column<string>(nullable: true),
                    Phone = table.Column<string>(nullable: false),
                    Email = table.Column<string>(nullable: true),
                    WorkAddressAddressId = table.Column<int>(nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_UserT", x => x.UserId);
                    table.ForeignKey(
                        name: "FK_UserT_AddressT_WorkAddressAddressId",
                        column: x => x.WorkAddressAddressId,
                        principalTable: "AddressT",
                        principalColumn: "AddressId",
                        onDelete: ReferentialAction.Restrict);
                });

            migrationBuilder.CreateTable(
                name: "AspNetRoleClaims",
                columns: table => new
                {
                    Id = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    RoleId = table.Column<string>(nullable: false),
                    ClaimType = table.Column<string>(nullable: true),
                    ClaimValue = table.Column<string>(nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_AspNetRoleClaims", x => x.Id);
                    table.ForeignKey(
                        name: "FK_AspNetRoleClaims_AspNetRoles_RoleId",
                        column: x => x.RoleId,
                        principalTable: "AspNetRoles",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "AspNetUserClaims",
                columns: table => new
                {
                    Id = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    UserId = table.Column<string>(nullable: false),
                    ClaimType = table.Column<string>(nullable: true),
                    ClaimValue = table.Column<string>(nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_AspNetUserClaims", x => x.Id);
                    table.ForeignKey(
                        name: "FK_AspNetUserClaims_AspNetUsers_UserId",
                        column: x => x.UserId,
                        principalTable: "AspNetUsers",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "AspNetUserLogins",
                columns: table => new
                {
                    LoginProvider = table.Column<string>(maxLength: 128, nullable: false),
                    ProviderKey = table.Column<string>(maxLength: 128, nullable: false),
                    ProviderDisplayName = table.Column<string>(nullable: true),
                    UserId = table.Column<string>(nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_AspNetUserLogins", x => new { x.LoginProvider, x.ProviderKey });
                    table.ForeignKey(
                        name: "FK_AspNetUserLogins_AspNetUsers_UserId",
                        column: x => x.UserId,
                        principalTable: "AspNetUsers",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "AspNetUserRoles",
                columns: table => new
                {
                    UserId = table.Column<string>(nullable: false),
                    RoleId = table.Column<string>(nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_AspNetUserRoles", x => new { x.UserId, x.RoleId });
                    table.ForeignKey(
                        name: "FK_AspNetUserRoles_AspNetRoles_RoleId",
                        column: x => x.RoleId,
                        principalTable: "AspNetRoles",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_AspNetUserRoles_AspNetUsers_UserId",
                        column: x => x.UserId,
                        principalTable: "AspNetUsers",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "AspNetUserTokens",
                columns: table => new
                {
                    UserId = table.Column<string>(nullable: false),
                    LoginProvider = table.Column<string>(maxLength: 128, nullable: false),
                    Name = table.Column<string>(maxLength: 128, nullable: false),
                    Value = table.Column<string>(nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_AspNetUserTokens", x => new { x.UserId, x.LoginProvider, x.Name });
                    table.ForeignKey(
                        name: "FK_AspNetUserTokens_AspNetUsers_UserId",
                        column: x => x.UserId,
                        principalTable: "AspNetUsers",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "PatientT",
                columns: table => new
                {
                    PatientID = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    UserId = table.Column<int>(nullable: false),
                    SocialSecurityNum = table.Column<string>(nullable: false),
                    InsuranceCompany = table.Column<int>(nullable: false),
                    HomeAddressAddressId = table.Column<int>(nullable: false),
                    HealthConditionId = table.Column<int>(nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_PatientT", x => x.PatientID);
                    table.ForeignKey(
                        name: "FK_PatientT_HealthConditionT_HealthConditionId",
                        column: x => x.HealthConditionId,
                        principalTable: "HealthConditionT",
                        principalColumn: "HealthConditionId",
                        onDelete: ReferentialAction.Restrict);
                    table.ForeignKey(
                        name: "FK_PatientT_AddressT_HomeAddressAddressId",
                        column: x => x.HomeAddressAddressId,
                        principalTable: "AddressT",
                        principalColumn: "AddressId",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "ActivityPricingPerDiagnosisT",
                columns: table => new
                {
                    ActivityPricingPerDiagnosisId = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    DiagnosisId = table.Column<int>(nullable: false),
                    MedicallActivityPriceId = table.Column<int>(nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_ActivityPricingPerDiagnosisT", x => x.ActivityPricingPerDiagnosisId);
                    table.ForeignKey(
                        name: "FK_ActivityPricingPerDiagnosisT_DiagnosisT_DiagnosisId",
                        column: x => x.DiagnosisId,
                        principalTable: "DiagnosisT",
                        principalColumn: "DiagnosisId",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_ActivityPricingPerDiagnosisT_MedicallActivityPriceT_MedicallActivityPriceId",
                        column: x => x.MedicallActivityPriceId,
                        principalTable: "MedicallActivityPriceT",
                        principalColumn: "MedicallActivityPriceId",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "MedicallBillT",
                columns: table => new
                {
                    MedicallBillId = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    DoctorId = table.Column<int>(nullable: false),
                    SocialSecurityNum = table.Column<string>(nullable: false),
                    MedicallActivityPriceId = table.Column<int>(nullable: false),
                    DiagnosisId = table.Column<int>(nullable: false),
                    State = table.Column<string>(nullable: true),
                    CreateDate = table.Column<DateTime>(nullable: false),
                    DecisionDate = table.Column<DateTime>(nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_MedicallBillT", x => x.MedicallBillId);
                    table.ForeignKey(
                        name: "FK_MedicallBillT_DiagnosisT_DiagnosisId",
                        column: x => x.DiagnosisId,
                        principalTable: "DiagnosisT",
                        principalColumn: "DiagnosisId",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_MedicallBillT_DoctorT_DoctorId",
                        column: x => x.DoctorId,
                        principalTable: "DoctorT",
                        principalColumn: "DoctorId",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_MedicallBillT_MedicallActivityPriceT_MedicallActivityPriceId",
                        column: x => x.MedicallActivityPriceId,
                        principalTable: "MedicallActivityPriceT",
                        principalColumn: "MedicallActivityPriceId",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "CheckupTicketT",
                columns: table => new
                {
                    CheckupTicketId = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    CreatedByDoctorId = table.Column<int>(nullable: false),
                    ToDoctorDoctorId = table.Column<int>(nullable: true),
                    PatientID = table.Column<int>(nullable: false),
                    Description = table.Column<string>(nullable: true),
                    IncludePicDescript = table.Column<bool>(nullable: false),
                    Result = table.Column<string>(nullable: true),
                    IncludePicRes = table.Column<bool>(nullable: false),
                    State = table.Column<string>(nullable: true),
                    CreateDate = table.Column<DateTime>(nullable: false),
                    FinishDate = table.Column<DateTime>(nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_CheckupTicketT", x => x.CheckupTicketId);
                    table.ForeignKey(
                        name: "FK_CheckupTicketT_DoctorT_CreatedByDoctorId",
                        column: x => x.CreatedByDoctorId,
                        principalTable: "DoctorT",
                        principalColumn: "DoctorId",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_CheckupTicketT_PatientT_PatientID",
                        column: x => x.PatientID,
                        principalTable: "PatientT",
                        principalColumn: "PatientID",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_CheckupTicketT_DoctorT_ToDoctorDoctorId",
                        column: x => x.ToDoctorDoctorId,
                        principalTable: "DoctorT",
                        principalColumn: "DoctorId",
                        onDelete: ReferentialAction.Restrict);
                });

            migrationBuilder.CreateTable(
                name: "MedicallReportT",
                columns: table => new
                {
                    MedicallReportId = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    AuthorDoctorId = table.Column<int>(nullable: false),
                    PatientID = table.Column<int>(nullable: false),
                    Description = table.Column<string>(nullable: true),
                    IncludePic = table.Column<bool>(nullable: false),
                    OwnerDoctorId = table.Column<int>(nullable: true),
                    CreateDate = table.Column<DateTime>(nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_MedicallReportT", x => x.MedicallReportId);
                    table.ForeignKey(
                        name: "FK_MedicallReportT_DoctorT_AuthorDoctorId",
                        column: x => x.AuthorDoctorId,
                        principalTable: "DoctorT",
                        principalColumn: "DoctorId",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_MedicallReportT_DoctorT_OwnerDoctorId",
                        column: x => x.OwnerDoctorId,
                        principalTable: "DoctorT",
                        principalColumn: "DoctorId",
                        onDelete: ReferentialAction.Restrict);
                    table.ForeignKey(
                        name: "FK_MedicallReportT_PatientT_PatientID",
                        column: x => x.PatientID,
                        principalTable: "PatientT",
                        principalColumn: "PatientID",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "PatientTreatmentLogT",
                columns: table => new
                {
                    PatientTreatmentLogId = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    PatientID = table.Column<int>(nullable: false),
                    DiagnosisId = table.Column<int>(nullable: false),
                    StartOfTreatment = table.Column<DateTime>(nullable: false),
                    EndOfTreatment = table.Column<DateTime>(nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_PatientTreatmentLogT", x => x.PatientTreatmentLogId);
                    table.ForeignKey(
                        name: "FK_PatientTreatmentLogT_DiagnosisT_DiagnosisId",
                        column: x => x.DiagnosisId,
                        principalTable: "DiagnosisT",
                        principalColumn: "DiagnosisId",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_PatientTreatmentLogT_PatientT_PatientID",
                        column: x => x.PatientID,
                        principalTable: "PatientT",
                        principalColumn: "PatientID",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "TicketPerDiagnosisT",
                columns: table => new
                {
                    TicketPerDiagnosisId = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    DiagnosisId = table.Column<int>(nullable: false),
                    CheckupTicketId = table.Column<int>(nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_TicketPerDiagnosisT", x => x.TicketPerDiagnosisId);
                    table.ForeignKey(
                        name: "FK_TicketPerDiagnosisT_CheckupTicketT_CheckupTicketId",
                        column: x => x.CheckupTicketId,
                        principalTable: "CheckupTicketT",
                        principalColumn: "CheckupTicketId",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_TicketPerDiagnosisT_DiagnosisT_DiagnosisId",
                        column: x => x.DiagnosisId,
                        principalTable: "DiagnosisT",
                        principalColumn: "DiagnosisId",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "CureProgressT",
                columns: table => new
                {
                    CureProgressId = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    MedicallReportId = table.Column<int>(nullable: false),
                    DiagnosisId = table.Column<int>(nullable: false),
                    StateOfTreatment = table.Column<string>(nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_CureProgressT", x => x.CureProgressId);
                    table.ForeignKey(
                        name: "FK_CureProgressT_DiagnosisT_DiagnosisId",
                        column: x => x.DiagnosisId,
                        principalTable: "DiagnosisT",
                        principalColumn: "DiagnosisId",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_CureProgressT_MedicallReportT_MedicallReportId",
                        column: x => x.MedicallReportId,
                        principalTable: "MedicallReportT",
                        principalColumn: "MedicallReportId",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "PictureT",
                columns: table => new
                {
                    PictureId = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    SocialSecurityNum = table.Column<string>(nullable: false),
                    CurrentPicture = table.Column<byte[]>(nullable: false),
                    Description = table.Column<string>(nullable: true),
                    ToMedicalReportMedicallReportId = table.Column<int>(nullable: true),
                    ToCheckupTicketCheckupTicketId = table.Column<int>(nullable: true),
                    CreateDate = table.Column<DateTime>(nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_PictureT", x => x.PictureId);
                    table.ForeignKey(
                        name: "FK_PictureT_CheckupTicketT_ToCheckupTicketCheckupTicketId",
                        column: x => x.ToCheckupTicketCheckupTicketId,
                        principalTable: "CheckupTicketT",
                        principalColumn: "CheckupTicketId",
                        onDelete: ReferentialAction.Restrict);
                    table.ForeignKey(
                        name: "FK_PictureT_MedicallReportT_ToMedicalReportMedicallReportId",
                        column: x => x.ToMedicalReportMedicallReportId,
                        principalTable: "MedicallReportT",
                        principalColumn: "MedicallReportId",
                        onDelete: ReferentialAction.Restrict);
                });

            migrationBuilder.CreateIndex(
                name: "IX_ActivityPricingPerDiagnosisT_DiagnosisId",
                table: "ActivityPricingPerDiagnosisT",
                column: "DiagnosisId");

            migrationBuilder.CreateIndex(
                name: "IX_ActivityPricingPerDiagnosisT_MedicallActivityPriceId",
                table: "ActivityPricingPerDiagnosisT",
                column: "MedicallActivityPriceId");

            migrationBuilder.CreateIndex(
                name: "IX_AspNetRoleClaims_RoleId",
                table: "AspNetRoleClaims",
                column: "RoleId");

            migrationBuilder.CreateIndex(
                name: "RoleNameIndex",
                table: "AspNetRoles",
                column: "NormalizedName",
                unique: true,
                filter: "[NormalizedName] IS NOT NULL");

            migrationBuilder.CreateIndex(
                name: "IX_AspNetUserClaims_UserId",
                table: "AspNetUserClaims",
                column: "UserId");

            migrationBuilder.CreateIndex(
                name: "IX_AspNetUserLogins_UserId",
                table: "AspNetUserLogins",
                column: "UserId");

            migrationBuilder.CreateIndex(
                name: "IX_AspNetUserRoles_RoleId",
                table: "AspNetUserRoles",
                column: "RoleId");

            migrationBuilder.CreateIndex(
                name: "EmailIndex",
                table: "AspNetUsers",
                column: "NormalizedEmail");

            migrationBuilder.CreateIndex(
                name: "UserNameIndex",
                table: "AspNetUsers",
                column: "NormalizedUserName",
                unique: true,
                filter: "[NormalizedUserName] IS NOT NULL");

            migrationBuilder.CreateIndex(
                name: "IX_CureProgressT_DiagnosisId",
                table: "CureProgressT",
                column: "DiagnosisId");

            migrationBuilder.CreateIndex(
                name: "IX_CureProgressT_MedicallReportId",
                table: "CureProgressT",
                column: "MedicallReportId");

            migrationBuilder.CreateIndex(
                name: "IX_CheckupTicketT_CreatedByDoctorId",
                table: "CheckupTicketT",
                column: "CreatedByDoctorId");

            migrationBuilder.CreateIndex(
                name: "IX_CheckupTicketT_PatientID",
                table: "CheckupTicketT",
                column: "PatientID");

            migrationBuilder.CreateIndex(
                name: "IX_CheckupTicketT_ToDoctorDoctorId",
                table: "CheckupTicketT",
                column: "ToDoctorDoctorId");

            migrationBuilder.CreateIndex(
                name: "IX_MedicallBillT_DiagnosisId",
                table: "MedicallBillT",
                column: "DiagnosisId");

            migrationBuilder.CreateIndex(
                name: "IX_MedicallBillT_DoctorId",
                table: "MedicallBillT",
                column: "DoctorId");

            migrationBuilder.CreateIndex(
                name: "IX_MedicallBillT_MedicallActivityPriceId",
                table: "MedicallBillT",
                column: "MedicallActivityPriceId");

            migrationBuilder.CreateIndex(
                name: "IX_MedicallReportT_AuthorDoctorId",
                table: "MedicallReportT",
                column: "AuthorDoctorId");

            migrationBuilder.CreateIndex(
                name: "IX_MedicallReportT_OwnerDoctorId",
                table: "MedicallReportT",
                column: "OwnerDoctorId");

            migrationBuilder.CreateIndex(
                name: "IX_MedicallReportT_PatientID",
                table: "MedicallReportT",
                column: "PatientID");

            migrationBuilder.CreateIndex(
                name: "IX_PatientT_HealthConditionId",
                table: "PatientT",
                column: "HealthConditionId");

            migrationBuilder.CreateIndex(
                name: "IX_PatientT_HomeAddressAddressId",
                table: "PatientT",
                column: "HomeAddressAddressId");

            migrationBuilder.CreateIndex(
                name: "IX_PatientTreatmentLogT_DiagnosisId",
                table: "PatientTreatmentLogT",
                column: "DiagnosisId");

            migrationBuilder.CreateIndex(
                name: "IX_PatientTreatmentLogT_PatientID",
                table: "PatientTreatmentLogT",
                column: "PatientID");

            migrationBuilder.CreateIndex(
                name: "IX_PictureT_ToCheckupTicketCheckupTicketId",
                table: "PictureT",
                column: "ToCheckupTicketCheckupTicketId");

            migrationBuilder.CreateIndex(
                name: "IX_PictureT_ToMedicalReportMedicallReportId",
                table: "PictureT",
                column: "ToMedicalReportMedicallReportId");

            migrationBuilder.CreateIndex(
                name: "IX_TicketPerDiagnosisT_CheckupTicketId",
                table: "TicketPerDiagnosisT",
                column: "CheckupTicketId");

            migrationBuilder.CreateIndex(
                name: "IX_TicketPerDiagnosisT_DiagnosisId",
                table: "TicketPerDiagnosisT",
                column: "DiagnosisId");

            migrationBuilder.CreateIndex(
                name: "IX_UserT_WorkAddressAddressId",
                table: "UserT",
                column: "WorkAddressAddressId");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "ActivityPricingPerDiagnosisT");

            migrationBuilder.DropTable(
                name: "AdminT");

            migrationBuilder.DropTable(
                name: "AspNetRoleClaims");

            migrationBuilder.DropTable(
                name: "AspNetUserClaims");

            migrationBuilder.DropTable(
                name: "AspNetUserLogins");

            migrationBuilder.DropTable(
                name: "AspNetUserRoles");

            migrationBuilder.DropTable(
                name: "AspNetUserTokens");

            migrationBuilder.DropTable(
                name: "CureProgressT");

            migrationBuilder.DropTable(
                name: "InsureEmpT");

            migrationBuilder.DropTable(
                name: "MedicallBillT");

            migrationBuilder.DropTable(
                name: "PatientTreatmentLogT");

            migrationBuilder.DropTable(
                name: "PictureT");

            migrationBuilder.DropTable(
                name: "TicketPerDiagnosisT");

            migrationBuilder.DropTable(
                name: "UserT");

            migrationBuilder.DropTable(
                name: "AspNetRoles");

            migrationBuilder.DropTable(
                name: "AspNetUsers");

            migrationBuilder.DropTable(
                name: "MedicallActivityPriceT");

            migrationBuilder.DropTable(
                name: "MedicallReportT");

            migrationBuilder.DropTable(
                name: "CheckupTicketT");

            migrationBuilder.DropTable(
                name: "DiagnosisT");

            migrationBuilder.DropTable(
                name: "DoctorT");

            migrationBuilder.DropTable(
                name: "PatientT");

            migrationBuilder.DropTable(
                name: "HealthConditionT");

            migrationBuilder.DropTable(
                name: "AddressT");
        }
    }
}
