/* One gentle entrance per content block, shared by Cindy's pages and guides. */
(() => {
  if (window.cindyRevealsLoaded) return;
  window.cindyRevealsLoaded = true;
  const motion = matchMedia('(prefers-reduced-motion: reduce)');
  if (!('IntersectionObserver' in window)) return;
  const style = document.createElement('style');
  style.textContent = `.site-reveal-ready{opacity:0;translate:0 28px;transition:opacity .65s ease,translate .65s cubic-bezier(.22,1,.36,1)}.site-reveal-ready.site-reveal-visible{opacity:1;translate:0 0}@media(prefers-reduced-motion:reduce){.site-reveal-ready{opacity:1;translate:none;transition:none}}`;
  document.head.append(style);
  const selectors = '.fcard,.bi,.club-inclusions-inner,.end-cards > .gcard,.wl-card,.end-line,.pcard,.bonus,.home-closing,.club-learning-intro,.club-learning-point,.sec-head,section > .wrap > .grid2,.guidebody > section,.guidebody > .promptwrap,.guidebody > .callout,section .contact,.pkg,.faq details,.contact-grid > div,.qa > .factcard,.qa > section,.il-section-intro,.il-steps > div,.il-topic-list > details,.il-end > .il-wrap,.ex-heading,.ex-gallery > .ex-card';
  const observer = new IntersectionObserver(entries => entries.forEach(entry => {
    if (!entry.isIntersecting) return;
    entry.target.classList.add('site-reveal-visible');
    observer.unobserve(entry.target);
  }), {threshold:.02,rootMargin:'0px 0px -20px 0px'});
  function scan(root=document) {
    if (motion.matches) return;
    const targets = [...root.querySelectorAll(selectors)];
    targets.forEach(el => {
      if (el.classList.contains('site-reveal-ready') || el.closest('.hero,.growth-ribbon,header,footer') || el.parentElement.closest('.site-reveal-ready')) return;
      const box=el.getBoundingClientRect();
      if (!box.width || !box.height || box.bottom < 0) return;
      el.classList.add('site-reveal-ready');
      observer.observe(el);
    });
  }
  function start(){
    scan();
    let frame;
    new MutationObserver(changes=>{
      if (!changes.some(change=>change.addedNodes.length)) return;
      cancelAnimationFrame(frame);frame=requestAnimationFrame(()=>scan());
    }).observe(document.body,{childList:true,subtree:true});
  }
  motion.addEventListener('change',()=>{
    if(motion.matches){document.querySelectorAll('.site-reveal-ready').forEach(el=>el.classList.add('site-reveal-visible'));observer.disconnect();}
    else scan();
  });
  document.addEventListener('focusin',e=>{const el=e.target.closest('.site-reveal-ready');if(el){el.classList.add('site-reveal-visible');observer.unobserve(el);}});
  if(document.readyState==='loading')document.addEventListener('DOMContentLoaded',start,{once:true});else start();
})();
