namespace AspNetCoreMVC
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);

            // Add services to the container.
            builder.Services.AddControllersWithViews();

            var app = builder.Build();

            // Configure the HTTP request pipeline.
            // � HTTP��û�� ������, ���� ��� ���������� ���� �Ϸ��� ����
            if (!app.Environment.IsDevelopment())
            {
                app.UseExceptionHandler("/Home/Error");
                // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
                app.UseHsts();
            }

            app.UseHttpsRedirection();
            app.UseStaticFiles(); // CSS, JS, �̹��� �� ��û�� ó���ϴ� �Լ�.

            app.UseRouting();

            app.UseAuthorization();

            // UseEndpoint?? -> ����� �̵����. ����� ���� �����Ѵ�.
            // ����� : ���� Ŭ���� ��û�� Ư�� MVC��Ʈ�ѷ��� �۾��� ��Ī�ϴ� ��.
            // https://localhost:7056/Home/Privacy
            // {controller=Home}     /{action=Index}/{id?}
            app.MapControllerRoute(
                name: "default",
                pattern: "{controller=Home}/{action=Index}/{id?}");

            app.Run();
        }
    }
}