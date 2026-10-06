#pragma once

// ============================================================
// WEB UI — served over real HTTPS by the ESP32 (self-signed cert).
// First visit: browser shows "connection not private" — tap
// Advanced -> Proceed. After that, this origin is fully secure
// to Chrome and the mic works normally, no flags/tricks needed.
// ============================================================

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Display Board</title>
<style>
  :root { --bg:#0a0f0a; --panel:#111811; --accent:#00e676; --dim:#5a6b5a; --danger:#ff5252; }
  * { box-sizing: border-box; }
  body { margin:0; font-family: -apple-system, Segoe UI, Roboto, sans-serif; background: var(--bg); color:#e8fbe8; min-height:100vh; }
  header { padding:18px 16px 8px; text-align:center; }
  header h1 { margin:0; font-size:1.2rem; letter-spacing:2px; color: var(--accent); }
  header p { margin:4px 0 0; font-size:0.75rem; color: var(--dim); }
  .tabs { display:flex; gap:4px; padding:12px 12px 0; position:sticky; top:0; background: var(--bg); z-index:10; }
  .tab { flex:1; padding:10px 4px; text-align:center; font-size:0.8rem; background: var(--panel); color: var(--dim); border-radius:10px 10px 0 0; cursor:pointer; border:1px solid #1f2b1f; border-bottom:none; }
  .tab.active { color: var(--accent); background:#152015; }
  .panel { display:none; background:#152015; margin:0 12px; padding:20px 16px 24px; border:1px solid #1f2b1f; border-radius:0 0 10px 10px; }
  .panel.active { display:block; }
  label { display:block; font-size:0.75rem; color: var(--dim); margin:14px 0 6px; }
  label:first-child { margin-top:0; }
  input[type=text], textarea, select { width:100%; padding:12px; font-size:1rem; background:#0d130d; border:1px solid #2a3a2a; border-radius:8px; color:#e8fbe8; }
  textarea { min-height:80px; resize:vertical; }
  .btn { width:100%; padding:14px; margin-top:16px; font-size:1rem; font-weight:600; border:none; border-radius:8px; cursor:pointer; background: var(--accent); color:#04220f; }
  .btn.secondary { background:#233023; color: var(--accent); }
  .btn.danger { background: var(--danger); color:#2a0000; }
  .btn.listening { background: var(--danger); color:#fff; }
  .btn:disabled { background:#2a3a2a; color: var(--dim); cursor:not-allowed; }
  .row { display:flex; gap:10px; }
  .row .btn { margin-top:16px; }
  .hint { font-size:0.72rem; color: var(--dim); margin-top:8px; line-height:1.4; }
  .warn { font-size:0.75rem; color:#ffcf5c; background:#2a2510; border:1px solid #4a3f10; padding:10px; border-radius:8px; margin-top:10px; line-height:1.4; }
  .status { font-size:0.8rem; margin-top:12px; min-height:1.2em; }
  .status.ok { color: var(--accent); }
  .status.err { color: var(--danger); }
  .soon-badge { display:inline-block; font-size:0.65rem; background:#2a3a2a; color: var(--dim); padding:2px 8px; border-radius:10px; margin-left:6px; vertical-align:middle; }
  .check-row { display:flex; align-items:center; gap:8px; margin-top:14px; font-size:0.85rem; color:#cfe; }
</style>
</head>
<body>

<header>
  <h1>DISPLAY BOARD</h1>
  <p>P10 sign · voice &amp; audio control</p>
</header>

<div class="tabs">
  <div class="tab active" data-tab="ttt">Display</div>
  <div class="tab" data-tab="stt">Voice</div>
  <div class="tab" data-tab="tts">Audio</div>
  <div class="tab" data-tab="sts">Talk</div>
</div>

<!-- ============ TTT: DISPLAY ============ -->
<div class="panel active" id="panel-ttt">
  <label for="ttt-line1">Top line</label>
  <input type="text" id="ttt-line1" placeholder="e.g. WELCOME" maxlength="40">
  <label for="ttt-line2">Bottom line</label>
  <input type="text" id="ttt-line2" placeholder="optional" maxlength="40">
  <button class="btn" onclick="sendDisplay('ttt')">Show on Board</button>
  <div class="status" id="ttt-status"></div>
</div>

<!-- ============ STT: VOICE ============ -->
<div class="panel" id="panel-stt">
  <label for="stt-text">Voice message</label>
  <textarea id="stt-text" placeholder="Tap the mic and speak"></textarea>

  <button class="btn" id="mic-btn" onclick="toggleMic()">🎤 Start Speaking</button>
  <div class="status" id="mic-status"></div>
  <div class="warn" id="mic-warn" style="display:none;"></div>

  <button class="btn" onclick="sendDisplay('stt')">Send to Board</button>
  <div class="status" id="stt-status"></div>
</div>

<!-- ============ TTS: AUDIO (placeholder) ============ -->
<div class="panel" id="panel-tts">
  <label>Announcement clip <span class="soon-badge">wiring soon</span></label>
  <select id="tts-name" disabled>
    <option>attention</option><option>welcome</option><option>classes</option>
    <option>assembly</option><option>break</option><option>endbreak</option>
    <option>closing</option><option>announcement</option><option>silence</option>
    <option>reminder</option><option>staff</option><option>visitors</option>
    <option>hall</option><option>alert</option><option>morning</option>
    <option>afternoon</option><option>evening</option><option>thankyou</option>
    <option>standby</option><option>greatday</option>
  </select>
  <div class="check-row"><input type="checkbox" id="tts-repeat" disabled> <label style="margin:0">Repeat</label></div>
  <div class="row">
    <button class="btn" disabled>Play</button>
    <button class="btn danger" disabled>Stop</button>
  </div>
  <div class="hint">Laid out and ready — wiring to the DFPlayer comes next.</div>
</div>

<!-- ============ STS: CONVERSATION (placeholder) ============ -->
<div class="panel" id="panel-sts">
  <label>Speech ⇄ Speech <span class="soon-badge">wiring soon</span></label>
  <button class="btn secondary" disabled>🎙️ Hold to Talk</button>
  <div class="hint">Needs a cloud round-trip, not wired yet — comes after Audio (TTS).</div>
</div>

<script>
  const API_BASE = "https://192.168.4.1"; // real TLS now — mic works directly here

  document.querySelectorAll('.tab').forEach(tab => {
    tab.addEventListener('click', () => {
      document.querySelectorAll('.tab').forEach(t => t.classList.remove('active'));
      document.querySelectorAll('.panel').forEach(p => p.classList.remove('active'));
      tab.classList.add('active');
      document.getElementById('panel-' + tab.dataset.tab).classList.add('active');
    });
  });

  function setStatus(id, msg, ok) {
    const el = document.getElementById(id);
    el.textContent = msg;
    el.className = 'status ' + (ok ? 'ok' : 'err');
  }

  async function postDisplay(line1, line2, statusId) {
    try {
      const res = await fetch(API_BASE + '/display', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ line1: line1, line2: line2 || '' })
      });
      const data = await res.json();
      if (res.ok && data.status === 'success') setStatus(statusId, 'Sent to board ✓', true);
      else setStatus(statusId, 'Board rejected it: ' + (data.message || res.status), false);
    } catch (e) {
      setStatus(statusId, 'Could not reach the board. Still on its Wi-Fi?', false);
    }
  }

  function sendDisplay(source) {
    if (source === 'ttt') {
      const l1 = document.getElementById('ttt-line1').value.trim();
      const l2 = document.getElementById('ttt-line2').value.trim();
      if (!l1 && !l2) { setStatus('ttt-status', 'Type something first.', false); return; }
      postDisplay(l1, l2, 'ttt-status');
    } else {
      const text = document.getElementById('stt-text').value.trim();
      if (!text) { setStatus('stt-status', 'Speak or type something first.', false); return; }
      postDisplay(text, '', 'stt-status');
    }
  }

  // ---- Real microphone speech-to-text (Web Speech API) ----
  // Note: Chrome's engine still calls out to Google for the actual
  // transcription step, so this tab needs the phone to have internet
  // at the moment of speaking (Wi-Fi or mobile data), same as any
  // other voice-typing feature. HTTPS here only fixes *permission*,
  // not the network hop recognition itself makes.
  const SR = window.SpeechRecognition || window.webkitSpeechRecognition;
  const micBtn = document.getElementById('mic-btn');
  const micWarn = document.getElementById('mic-warn');
  let recognizer = null;
  let listening = false;

  if (!SR) {
    micBtn.disabled = true;
    micBtn.textContent = '🎤 Mic not supported in this browser';
  } else {
    recognizer = new SR();
    recognizer.continuous = false;
    recognizer.interimResults = true;
    recognizer.lang = 'en-US';

    recognizer.onresult = (e) => {
      let text = '';
      for (let i = 0; i < e.results.length; i++) text += e.results[i][0].transcript;
      document.getElementById('stt-text').value = text;
    };
    recognizer.onerror = (e) => {
      if (e.error === 'network') {
        micWarn.style.display = 'block';
        micWarn.textContent = 'No route to the recognition service right now — the phone needs internet (Wi-Fi or mobile data) at the moment you speak, even though sending the result to the board only needs the board\'s own Wi-Fi.';
      } else if (e.error === 'not-allowed') {
        micWarn.style.display = 'block';
        micWarn.textContent = 'Microphone permission was blocked. Tap the lock icon in the address bar and allow Microphone for this site, then try again.';
      } else {
        setStatus('mic-status', 'Mic error: ' + e.error, false);
      }
    };
    recognizer.onend = () => {
      listening = false;
      micBtn.textContent = '🎤 Start Speaking';
      micBtn.classList.remove('listening');
    };
  }

  function toggleMic() {
    if (!recognizer) return;
    micWarn.style.display = 'none';
    if (listening) {
      recognizer.stop();
    } else {
      document.getElementById('stt-text').value = '';
      setStatus('mic-status', 'Listening…', true);
      recognizer.start();
      listening = true;
      micBtn.textContent = '⏹ Stop';
      micBtn.classList.add('listening');
    }
  }
</script>

</body>
</html>
)rawliteral";
