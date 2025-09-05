#ifndef WEB_STYLES_H
#define WEB_STYLES_H

const char CSS[] PROGMEM = R"(
*{margin:0;padding:0;box-sizing:border-box}
body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif;background:#0f0f23;color:#cccccc;line-height:1.6}
.container{max-width:800px;margin:0 auto;padding:20px}
.header{text-align:center;margin-bottom:30px;border-bottom:2px solid #10101a;padding-bottom:20px}
.header h1{color:#00ff41;font-size:28px;font-weight:300;letter-spacing:2px}
.header p{color:#666;font-size:14px;margin-top:5px}
.grid{display:grid;grid-template-columns:1fr 1fr;gap:20px;margin-bottom:20px}
@media(max-width:768px){.grid{grid-template-columns:1fr}}
.card{background:#1a1a2e;border:1px solid #16213e;border-radius:8px;padding:20px;box-shadow:0 4px 6px rgba(0,0,0,0.3)}
.card h3{color:#00ff41;margin-bottom:15px;font-size:16px;font-weight:500}
.form-group{margin-bottom:15px}
label{display:block;color:#aaa;font-size:13px;margin-bottom:5px;font-weight:500}
select,input,textarea{width:100%;padding:12px;background:#0f0f23;border:1px solid #333;border-radius:6px;color:#ccc;font-size:14px}
select:focus,input:focus,textarea:focus{outline:none;border-color:#00ff41;box-shadow:0 0 0 2px rgba(0,255,65,0.1)}
.btn{width:100%;padding:12px;border:none;border-radius:6px;font-size:14px;font-weight:500;cursor:pointer;transition:all 0.2s;margin:5px 0}
.btn-primary{background:#00ff41;color:#000}
.btn-primary:hover{background:#00cc33;transform:translateY(-1px)}
.btn-danger{background:#ff4757;color:#fff}
.btn-danger:hover{background:#ff3742;transform:translateY(-1px)}
.btn-secondary{background:#2f3542;color:#ccc;border:1px solid #57606f}
.btn-secondary:hover{background:#57606f}
.status{background:#16213e;border-left:4px solid #00ff41;padding:15px;margin-bottom:20px;border-radius:0 6px 6px 0}
.status-item{display:flex;justify-content:space-between;margin:5px 0}
.status-label{color:#aaa}
.status-value{color:#00ff41;font-weight:500}
.attack-controls{display:grid;grid-template-columns:repeat(auto-fit,minmax(200px,1fr));gap:10px;margin-top:20px}
textarea{resize:vertical;min-height:100px}
.info{font-size:12px;color:#666;text-align:center;margin-top:10px}
)";

#endif
